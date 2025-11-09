#include "login_dialog.h"

#include <qjsondocument.h>

#include <QFile>
#include <QFont>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QString>
#include <QWidget>

#include "net/http_mgr.h"
#include "net/tcp_mgr.h"
#include "utils/util.h"


// TODO: 登录功能
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
	SetUI();
	InitHttpHandler();

	// TODO: initHead(): 加载已注册用户的头像

	// 跳转到注册界面
	connect(register_button_, &QPushButton::clicked, this,
			&LoginDialog::SigSwitchRegister);
	// 登录功能
	connect(login_button_, &QPushButton::clicked, this, &LoginDialog::SlotLogin);
	// 登录模块完成槽函数
	connect(HttpMgr::GetInstance().get(), &HttpMgr::SigLoginModFinish, this,
			&LoginDialog::SlotLoginModFinish);
	// 连接tcp连接请求的信号和槽函数
	connect(this, &LoginDialog::SigConnectTcp, TcpMgr::GetInstance().get(),
			&TcpMgr::SlotTcpConnect);
	// 连接tcp管理者发出的连接成功信号
	connect(TcpMgr::GetInstance().get(), &TcpMgr::SigConnectSuccess, this,
			&LoginDialog::SlotTcpConnectFinish);
}

// 注册请求回调函数
void LoginDialog::InitHttpHandler() {
	// 登录模块完成槽函数
	handlers_[ReqId::LOGIN_USER] = [this](const QJsonObject &json_obj) {
		int error = json_obj["error"].toInt();
		if (error != static_cast<int>(ErrorCodes::SUCCESS)) {
			ShowErrorTip(tr("参数错误"), false);
			EnableBtn(true);
			return;
		}
		std::cout << "success receive data" << std::endl;
		auto email = json_obj["email"].toString();
		// 发送信号通知tcpMgr发送长链接
		ServerInfo si;
		// si.Uid = json_obj["user_id"].toString();
		// si.Host = json_obj["host"].toString();
		// si.Port = json_obj["port"].toString();
		// si.Token = json_obj["token"].toString();
		si.Uid = "769589943935700992";
		si.Host = "127.0.0.1";
		si.Port = "8090";
		si.Token = "ed2ecf3b-8ba6-4906-b15e-0565a4fb685d";

		uid_ = si.Uid;
		token_ = si.Token;

		// qDebug() << "email is " << email << " uid is " << si.Uid << " host is " <<
		// si.Host
		// 		 << " Port is " << si.Port << " Token is " << si.Token;

		emit SigConnectTcp(si);
	};
}

void LoginDialog::Login(){
	// 发送信号通知tcpMgr发送长链接
	ServerInfo si;
	si.Uid = "769589943935700992";
	// si.Uid = "765575560830128128";
	si.Host = "127.0.0.1";
	si.Port = "8090";
	si.Token = "ed2ecf3b-8ba6-4906-b15e-0565a4fb685d";
	// si.Token = "4b8c797d-58d1-4610-a770-bb6665cbc7e0";

	uid_ = si.Uid;
	token_ = si.Token;
	
	emit SigConnectTcp(si);
}

// 设置用户登录时, 登录按钮和注册按钮是否可用
bool LoginDialog::EnableBtn(bool enabled) {
	login_button_->setEnabled(enabled);
	register_button_->setEnabled(enabled);
	return true;
}

// 用户登录功能
void LoginDialog::SlotLogin() {
	if (!SlotCheckEmail()) {
		return;
	}

	if (password_lineedit_->text().isEmpty()) {
		return;
	}

	EnableBtn(false);
	auto email = email_lineedit_->text();
	auto pwd = password_lineedit_->text();
	// 发送http请求登录
	QJsonObject json_obj;
	json_obj["email"] = email;
	json_obj["password"] = xorString(pwd);
	HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_login"), json_obj,
										ReqId::LOGIN_USER, Modules::LOGINMOD);
}

// 登录模块完成槽函数
void LoginDialog::SlotLoginModFinish(ReqId id, QString res, ErrorCodes err) {
	if (err != ErrorCodes::SUCCESS) {
		ShowErrorTip(tr("网络请求错误"), false);
		return;
	}

	// 解析 JSON 字符串,res需转化为QByteArray
	QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
	// json解析错误
	if (jsonDoc.isNull()) {
		ShowErrorTip(tr("json解析错误"), false);
		return;
	}

	// json解析错误
	if (!jsonDoc.isObject()) {
		ShowErrorTip(tr("json解析错误"), false);
		return;
	}

	std::cout << "call caller" << std::endl;

	// 调用对应的逻辑,根据id回调。
	handlers_[id](jsonDoc.object());

	return;
}

// 连接tcp管理者发出的连接成功信号
void LoginDialog::SlotTcpConnectFinish(bool bsuccess) {
	if (bsuccess) {
		ShowErrorTip(tr("聊天服务连接成功，正在登录..."), true);
		QJsonObject jsonObj;
		jsonObj["uid"] = uid_;
		jsonObj["token"] = token_;

		QJsonDocument doc(jsonObj);
		QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

		// 发送tcp请求给chat server, 正式登录ChatServer
		emit TcpMgr::GetInstance()->SigSendData(ReqId::CHAT_LOGIN, jsonData);

	} else {
		ShowErrorTip(tr("网络异常"), false);
		EnableBtn(true);
	}
}

// 验证邮箱是否正确
bool LoginDialog::SlotCheckEmail() {
	QString email = email_lineedit_->text();
	if (email.isEmpty()) {
		AddTipErr(TipErr::ERR_EMAIL, tr("邮箱不能为空"));
		return false;
	}

	DelTipErr(TipErr::ERR_EMAIL);
	return true;
}

// 验证密码
bool LoginDialog::SlotCheckPassword() {
	auto pass = password_lineedit_->text();

	if (pass.length() < 6 || pass.length() > 15) {
		qDebug() << "Pass length invalid";
		// 提示长度不准确
		AddTipErr(TipErr::ERR_PASSWORD, tr("密码长度应为6~15"));
		return false;
	}

	// 创建一个正则表达式对象，按照上述密码要求
	// 这个正则表达式解释：
	// ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
	QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
	bool match = regExp.match(pass).hasMatch();
	if (!match) {
		// 提示字符非法
		AddTipErr(TipErr::ERR_PASSWORD, tr("不能包含非法字符且长度为(6~15)"));
		return false;
		;
	}

	DelTipErr(TipErr::ERR_PASSWORD);

	return true;

	return true;
}

// 添加已存在的错误提示
void LoginDialog::AddTipErr(TipErr te, QString tips) {
	tip_errs_[te] = tips;
	ShowErrorTip(tips, false);
}

// 删除已存在的错误提示
void LoginDialog::DelTipErr(TipErr te) {
	tip_errs_.remove(te);
	if (tip_errs_.empty()) {
		error_label_->clear();
		return;
	}

	ShowErrorTip(tip_errs_.first(), false);
}

// 根据请求的正确与否设置 error_label_ 的样式和文本
void LoginDialog::ShowErrorTip(QString str, bool b_ok) {
	if (b_ok) {
		error_label_->setProperty("state", "normal");
	} else {
		error_label_->setProperty("state", "err");
	}

	error_label_->setText(str);

	repolish(error_label_);
}

void LoginDialog::SetUI() {
	// 实现登录界面
	// 窗口标题
	this->setWindowTitle("用户登录");
	// 窗口大小
	this->setFixedSize(1000, 500);

	// 字体
	QFont font("微软雅黑", 12);
	font.setPointSize(12);	// 设置字体大小为12

	// 错误提示
	error_label_ = new QLabel(this);
	error_label_->setFont(font);
	error_label_->setStyleSheet("color: red;");

	email_label_ = new QLabel("邮箱:", this);
	email_label_->setFont(font);
	password_label_ = new QLabel("密码:", this);
	password_label_->setFont(font);
	// 邮箱输入框
	email_lineedit_ = new QLineEdit(this);
	email_lineedit_->setFont(font);
	email_lineedit_->setPlaceholderText("请输入邮箱");
	// 密码输入框
	password_lineedit_ = new QLineEdit(this);
	password_lineedit_->setFont(font);
	password_lineedit_->setEchoMode(QLineEdit::Password);  // 设置密码输入框为密码模式
	password_lineedit_->setPlaceholderText("请输入密码");

	// 登录按钮
	login_button_ = new QPushButton("登录", this);
	login_button_->setFont(font);
	login_button_->setMinimumHeight(40);

	// 注册按钮
	register_button_ = new QPushButton("注册", this);
	register_button_->setFont(font);
	register_button_->setMinimumHeight(40);

	// 创建布局
	QVBoxLayout *main_layout = new QVBoxLayout(this);
	main_layout->setSpacing(20);
	// 设置布局的外边距
	main_layout->setContentsMargins(100, 30, 100, 30);

	// 错误提示
	main_layout->addWidget(error_label_);

	// 邮箱输入框布局
	QHBoxLayout *email_layout = new QHBoxLayout();
	email_layout->addWidget(email_label_);
	email_layout->addWidget(email_lineedit_);
	main_layout->addLayout(email_layout);

	// 密码输入框布局
	QHBoxLayout *password_layout = new QHBoxLayout();
	password_layout->addWidget(password_label_);
	password_layout->addWidget(password_lineedit_);
	main_layout->addLayout(password_layout);

	// 设置登录界面的按钮居中对齐
	// QHBoxLayout *button_layout = new QHBoxLayout();
	main_layout->addStretch();
	main_layout->addWidget(login_button_);
	main_layout->addStretch();	// 添加一个弹性空间，使按钮居中对齐
	// main_layout->addLayout(button_layout);

	// 注册按钮
	main_layout->addStretch();
	main_layout->addWidget(register_button_);
	main_layout->addStretch();	// 添加一个弹性空间，使按钮居中对齐

	// 设置登录对话框的布局
	this->setLayout(main_layout);

	// 设置控件的名称
	login_button_->setObjectName("login_button");
	// 加载 qss
	LoadStyleSheet(this, "../style/login_dialog.qss");
}
