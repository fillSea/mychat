#include "register_dialog.h"

#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qwindowdefs.h>

#include <QFile>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QTimer>
#include <QWidget>
#include <QDebug>

#include "net/http_mgr.h"
#include "utils/util.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), countdown_(5) {
	SetUI();
	SetQssName();
	LoadStyleSheet(this, "../style/register_dialog.qss");

	password_show_label_->SetState("unvisible", "unvisible_hover", "", "visible",
								   "visible_hover", "");
	confirm_password_show_label_->SetState("unvisible", "unvisible_hover", "", "visible",
										   "visible_hover", "");

	InitHttpHandler();

	// 发送切换回登录界面信号
	connect(cancel_button_, &QPushButton::clicked, this, &RegisterDialog::SigSwitchLogin);

	// 验证输入是否正确
	// 验证用户名是否正确
	connect(username_lineedit_, &QLineEdit::editingFinished, this,
			[this]() { SlotCheckUsername(); });
	// 验证邮箱是否正确
	connect(get_verify_code_button_, &QPushButton::clicked, this,
			&RegisterDialog::SlotCheckEmail);
	// 验证密码是否正确
	connect(password_lineedit_, &QLineEdit::editingFinished, this,
			[this]() { SlotCheckPassword(); });
	// 验证重复密码是否正确
	connect(confirm_password_lineedit_, &QLineEdit::editingFinished, this,
			[this]() { SlotCheckConfirmPassword(); });
	// 验证验证码是否正确
	connect(verify_code_lineedit_, &QLineEdit::editingFinished, this,
			[this]() { SlotCheckVerifyCode(); });

	// 密码显示与隐藏
	connect(password_show_label_, &ClickedLabel::SigClicked, this, [this]() {
		auto state = password_show_label_->GetCurState();
		if (state == ClickLbState::Normal) {
			password_lineedit_->setEchoMode(QLineEdit::Password);
		} else {
			password_lineedit_->setEchoMode(QLineEdit::Normal);
		}
	});

	connect(confirm_password_show_label_, &ClickedLabel::SigClicked, this, [this]() {
		auto state = confirm_password_show_label_->GetCurState();
		if (state == ClickLbState::Normal) {
			confirm_password_lineedit_->setEchoMode(QLineEdit::Password);
		} else {
			confirm_password_lineedit_->setEchoMode(QLineEdit::Normal);
		}
	});

	// 注册页面倒计时定时器
	// 创建定时器
	countdown_timer_ = new QTimer(this);
	// 连接信号和槽
	connect(countdown_timer_, &QTimer::timeout, [this]() {
		if (countdown_ == 0) {
			countdown_timer_->stop();
			emit SigSwitchLogin();
			return;
		}
		countdown_--;
		auto str = QString("注册成功，%1 s后返回登录").arg(countdown_);
		success_label_->setText(str);
	});

	// 获取验证码
	connect(get_verify_code_button_, &QPushButton::clicked, this,
			&RegisterDialog::SlotGetVerifycode);
	// 用户注册
	connect(register_button_, &QPushButton::clicked, this,
			&RegisterDialog::SlotRegisterUser);
	// 注册请求完成回调函数
	connect(HttpMgr::GetInstance().get(), &HttpMgr::SigRegModFinish, this,
			&RegisterDialog::SlotRegModFinish);
}

// 注册请求回调函数
void RegisterDialog::InitHttpHandler() {
	// 注册获取验证码回包逻辑
	handlers_.insert(ReqId::GET_VARIFY_CODE, [this](QJsonObject jsonObj) {
		int error = jsonObj["error"].toInt();
		if (error != static_cast<int>(ErrorCodes::SUCCESS)) {
			ShowErrorTip(tr("参数错误"), false);
			return;
		}
		auto email = jsonObj["email"].toString();
		ShowErrorTip(tr("验证码已发送到邮箱，注意查收"), true);
	});

	// 注册用户回包逻辑
	handlers_.insert(ReqId::REG_USER, [this](QJsonObject jsonObj) {
		int error = jsonObj["error"].toInt();
		if (error != static_cast<int>(ErrorCodes::SUCCESS)) {
			ShowErrorTip(tr("参数错误"), false);
			return;
		}
		auto email = jsonObj["email"].toString();
		ShowErrorTip(tr("用户注册成功"), true);
		qDebug() << "email is " << email;
		qDebug() << "user uuid is " << jsonObj["user_id"].toString();
		ChangeTipPage();
	});
}

// 获取验证码
void RegisterDialog::SlotGetVerifycode() {
	// 验证邮箱的地址正则表达式
	auto email = email_lineedit_->text();
	bool valid = SlotCheckEmail();
	if (valid) {
		// 发送http请求获取验证码
		QJsonObject json_obj;
		json_obj["email"] = email;
		HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"),
											json_obj, ReqId::GET_VARIFY_CODE,
											Modules::REGISTERMOD);
	}
}

// 注册用户功能
void RegisterDialog::SlotRegisterUser() {
	// 参数验证
	bool valid = SlotCheckUsername();
	if (!valid) {
		return;
	}

	valid = SlotCheckEmail();
	if (!valid) {
		return;
	}

	valid = SlotCheckPassword();
	if (!valid) {
		return;
	}

	valid = SlotCheckConfirmPassword();
	if (!valid) {
		return;
	}

	valid = SlotCheckVerifyCode();
	if (!valid) {
		return;
	}

	// 发送注册请求
	QJsonObject json_obj;
	json_obj["username"] = username_lineedit_->text();
	json_obj["email"] = email_lineedit_->text();
	json_obj["password"] = xorString(password_lineedit_->text());
	json_obj["confirm_password"] = xorString(confirm_password_lineedit_->text());
	json_obj["verifycode"] = verify_code_lineedit_->text();

	// 随机获取头像
	int randomValue =
		QRandomGenerator::global()->bounded(100);  // 生成0到99之间的随机整数
	int head_i = randomValue % heads.size();
	json_obj["icon"] = heads[head_i];

	HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
										json_obj, ReqId::REG_USER, Modules::REGISTERMOD);
}

// 注册模块完成槽函数
void RegisterDialog::SlotRegModFinish(ReqId id, QString res, ErrorCodes err) {
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

	// 调用对应的逻辑,根据id回调。
	handlers_[id](jsonDoc.object());

	return;
}

// 切换到注册成功提示页面
void RegisterDialog::ChangeTipPage() {
	countdown_timer_->stop();

	// 设置注册成功提示页面
	stacked_widget_->setCurrentWidget(success_page_);

	// 启动定时器，设置间隔为1000毫秒（1秒）
	countdown_timer_->start(1000);
}

// 验证输入是否正确
// 验证用户名
bool RegisterDialog::SlotCheckUsername() {
	if (username_lineedit_->text() == "") {
		AddTipErr(TipErr::ERR_USERNAME, tr("用户名不能为空"));
		return false;
	}

	DelTipErr(TipErr::ERR_USERNAME);
	return true;
}

// 验证邮箱是否正确
bool RegisterDialog::SlotCheckEmail() {
	QString email = email_lineedit_->text();
	if (email.isEmpty()) {
		AddTipErr(TipErr::ERR_EMAIL, tr("请输入邮箱"));
		return false;
	}

	// 邮箱地址的正则表达式
	QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
	// 验证邮箱是否符合正则表达式
	bool is_match = regex.match(email).hasMatch();
	if (!is_match) {
		AddTipErr(TipErr::ERR_EMAIL, tr("邮箱地址不正确"));
		return false;
	}
	DelTipErr(TipErr::ERR_EMAIL);
	return true;
}

// 验证密码
bool RegisterDialog::SlotCheckPassword() {
	auto pass = password_lineedit_->text();
	auto confirm = confirm_password_lineedit_->text();

	if (pass.length() < 6 || pass.length() > 15) {
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
		AddTipErr(TipErr::ERR_PASSWORD, tr("不能包含非法字符"));
		return false;
		;
	}

	DelTipErr(TipErr::ERR_PASSWORD);

	if (pass != confirm) {
		// 提示密码不匹配
		AddTipErr(TipErr::ERR_CONFIRM_PASSWORD, tr("密码和确认密码不匹配"));
		return false;
	} else {
		DelTipErr(TipErr::ERR_CONFIRM_PASSWORD);
	}
	return true;
}

// 验证确认密码
bool RegisterDialog::SlotCheckConfirmPassword() {
	auto pass = password_lineedit_->text();
	auto confirm = confirm_password_lineedit_->text();

	if (confirm.length() < 6 || confirm.length() > 15) {
		// 提示长度不准确
		AddTipErr(TipErr::ERR_CONFIRM_PASSWORD, tr("确认密码长度应为6~15"));
		return false;
	}

	// 创建一个正则表达式对象，按照上述密码要求
	// 这个正则表达式解释：
	// ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
	QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
	bool match = regExp.match(confirm).hasMatch();
	if (!match) {
		// 提示字符非法
		AddTipErr(TipErr::ERR_CONFIRM_PASSWORD, tr("不能包含非法字符"));
		return false;
	}

	DelTipErr(TipErr::ERR_CONFIRM_PASSWORD);

	if (pass != confirm) {
		// 提示密码不匹配
		AddTipErr(TipErr::ERR_CONFIRM_PASSWORD, tr("确认密码和密码不匹配"));
		return false;
	} else {
		DelTipErr(TipErr::ERR_CONFIRM_PASSWORD);
	}
	return true;
}

// 验证验证码
bool RegisterDialog::SlotCheckVerifyCode() {
	auto pass = verify_code_lineedit_->text();
	if (pass.isEmpty()) {
		AddTipErr(TipErr::ERR_VERIFYCODE, tr("验证码不能为空"));
		return false;
	}

	DelTipErr(TipErr::ERR_VERIFYCODE);
	return true;
}

// 添加已存在的错误提示
void RegisterDialog::AddTipErr(TipErr te, QString tips) {
	tip_errs_[te] = tips;
	ShowErrorTip(tips, false);
}

// 删除已存在的错误提示
void RegisterDialog::DelTipErr(TipErr te) {
	tip_errs_.remove(te);
	if (tip_errs_.empty()) {
		error_label_->clear();
		return;
	}

	ShowErrorTip(tip_errs_.first(), false);
}

// 根据请求的正确与否设置 error_label_ 的样式和文本
void RegisterDialog::ShowErrorTip(QString str, bool b_ok) {
	if (b_ok) {
		error_label_->setProperty("state", "normal");
	} else {
		error_label_->setProperty("state", "err");
	}

	error_label_->setText(str);

	repolish(error_label_);
}

void RegisterDialog::SetUI() {
	// 实现登录界面
	// 窗口标题
	this->setWindowTitle(tr("注册"));
	// 窗口大小
	this->setFixedSize(1000, 500);

	// 字体
	QFont font("微软雅黑", 12);
	font.setPointSize(12);	// 设置字体大小为12

	// 创建堆叠窗口
    stacked_widget_ = new QStackedWidget(this);
	// 创建注册表单页面
    register_page_ = new QWidget(stacked_widget_);

	// 错误提示
	error_label_ = new QLabel(register_page_);
	error_label_->setFont(font);
	error_label_->setStyleSheet("color: red;");

	// 用户名
	username_label_ = new QLabel("用户名:", register_page_);
	username_label_->setFont(font);
	// 用户名输入框
	username_lineedit_ = new QLineEdit(register_page_);
	username_lineedit_->setFont(font);
	username_lineedit_->setPlaceholderText("请输入用户名");

	email_label_ = new QLabel("邮箱:", register_page_);
	email_label_->setFont(font);
	// 邮箱输入框
	email_lineedit_ = new QLineEdit(register_page_);
	email_lineedit_->setFont(font);
	email_lineedit_->setPlaceholderText("请输入邮箱");

	// 密码
	password_label_ = new QLabel("密码:", register_page_);
	password_label_->setFont(font);
	// 密码输入框
	password_lineedit_ = new QLineEdit(register_page_);
	password_lineedit_->setFont(font);
	password_lineedit_->setEchoMode(QLineEdit::Password);  // 设置密码输入框为密码模式
	password_lineedit_->setPlaceholderText("请输入密码");

	// 重复密码
	confirm_password_label_ = new QLabel("重复密码:", register_page_);
	confirm_password_label_->setFont(font);
	// 重复密码输入框
	confirm_password_lineedit_ = new QLineEdit(register_page_);
	confirm_password_lineedit_->setFont(font);
	confirm_password_lineedit_->setEchoMode(
		QLineEdit::Password);  // 设置密码输入框为密码模式
	confirm_password_lineedit_->setPlaceholderText("请重复输入密码");

	// 密码后面的显示/隐藏标签
	password_show_label_ = new ClickedLabel(register_page_);
	password_show_label_->setFont(font);
	// 设置浮动显示手形状
	password_show_label_->setCursor(Qt::PointingHandCursor);

	// 重复密码后面的显示/隐藏标签
	confirm_password_show_label_ = new ClickedLabel(register_page_);
	confirm_password_show_label_->setFont(font);
	confirm_password_show_label_->setCursor(Qt::PointingHandCursor);

	// 验证码
	verify_code_label_ = new QLabel("验证码:", register_page_);
	verify_code_label_->setFont(font);
	// 验证码输入框
	verify_code_lineedit_ = new QLineEdit(register_page_);
	verify_code_lineedit_->setFont(font);
	verify_code_lineedit_->setPlaceholderText("请输入验证码");

	// 获取验证码按钮
	get_verify_code_button_ = new TimerBtn(register_page_);
	get_verify_code_button_->setText("获取");
	get_verify_code_button_->setFont(font);
	get_verify_code_button_->setMinimumHeight(40);

	// 注册按钮
	register_button_ = new QPushButton("注册", register_page_);
	register_button_->setFont(font);
	register_button_->setMinimumHeight(40);

	// 取消按钮, 点击后切换回登录界面
	cancel_button_ = new QPushButton("取消", register_page_);
	cancel_button_->setFont(font);
	cancel_button_->setMinimumHeight(40);

	// 创建布局
	QVBoxLayout *register_layout = new QVBoxLayout(register_page_);
	register_layout->setSpacing(20);
	// 设置布局的外边距
	register_layout->setContentsMargins(100, 30, 100, 30);
	// 添加错误提示
	register_layout->addWidget(error_label_);

	// 用户名输入框布局
	QHBoxLayout *username_layout = new QHBoxLayout();
	username_layout->addWidget(username_label_);
	username_layout->addWidget(username_lineedit_);
	register_layout->addLayout(username_layout);

	// 邮箱输入框布局
	QHBoxLayout *email_layout = new QHBoxLayout();
	email_layout->addWidget(email_label_);
	email_layout->addWidget(email_lineedit_);
	register_layout->addLayout(email_layout);

	// 密码输入框布局
	QHBoxLayout *password_layout = new QHBoxLayout();
	password_layout->addWidget(password_label_);
	password_layout->addWidget(password_lineedit_);
	password_show_label_->setFixedSize(40, 40);
	password_layout->addWidget(password_show_label_);
	register_layout->addLayout(password_layout);

	// 重复密码输入框布局
	QHBoxLayout *confirm_password_layout = new QHBoxLayout();
	confirm_password_layout->addWidget(confirm_password_label_);
	confirm_password_layout->addWidget(confirm_password_lineedit_);
	confirm_password_show_label_->setFixedSize(40, 40);
	confirm_password_layout->addWidget(confirm_password_show_label_);
	register_layout->addLayout(confirm_password_layout);

	// 验证码输入框布局
	QHBoxLayout *verify_code_layout = new QHBoxLayout();
	verify_code_layout->addWidget(verify_code_label_);
	verify_code_layout->addWidget(verify_code_lineedit_);
	register_layout->addLayout(verify_code_layout);
	// 获取验证码按钮布局
	verify_code_layout->addWidget(get_verify_code_button_);

	// 设置注册界面的按钮居中对齐
	register_layout->addStretch();
	register_layout->addWidget(register_button_);
	register_layout->addStretch();	// 添加一个弹性空间，使按钮居中对齐

	// 取消按钮布局
	register_layout->addStretch();
	register_layout->addWidget(cancel_button_);
	register_layout->addStretch();	// 添加一个弹性空间，使按钮居中对齐

	// 设置注册对话框的布局
	register_page_->setLayout(register_layout);

	// 设置注册成功提示页面
	// 初始化成功页面
	success_page_ = new QWidget(stacked_widget_);

	// 设置成功页面布局
	QVBoxLayout *success_layout = new QVBoxLayout(success_page_);
	success_label_ = new QLabel(success_page_);
	success_label_->setFont(font);
	success_label_->setAlignment(Qt::AlignCenter);
	success_layout->addWidget(success_label_);
	success_page_->setLayout(success_layout);

	// 主窗口页面布局
	QVBoxLayout *main_layout = new QVBoxLayout(this);
	main_layout->addWidget(stacked_widget_);
	// 将两个页面添加到堆叠窗口
    stacked_widget_->addWidget(register_page_);
    stacked_widget_->addWidget(success_page_);
    
    // 默认显示注册页面
    stacked_widget_->setCurrentWidget(register_page_);

	this->setLayout(main_layout);
}

// 设置 qss 样式名称
void RegisterDialog::SetQssName() {
	error_label_->setObjectName(("err_tip"));
	password_show_label_->setObjectName(("pass_visible"));
	confirm_password_show_label_->setObjectName(("confirm_visible"));
}
