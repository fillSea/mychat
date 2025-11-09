#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "const.h"

// 登录
class LoginDialog : public QDialog {
	Q_OBJECT
public:
	explicit LoginDialog(QWidget *parent = nullptr);
	void Login();
signals:
	void SigSwitchRegister();  // 切换到注册界面
	// 连接聊天服务器
	void SigConnectTcp(ServerInfo si);
private slots:
	// 用户登录功能
	void SlotLogin();
	// 验证邮箱是否正确
	bool SlotCheckEmail();
	// 验证密码
	bool SlotCheckPassword();
	// 登录模块完成槽函数
	void SlotLoginModFinish(ReqId id, QString res, ErrorCodes err);
	// 连接tcp管理者发出的连接成功信号
	void SlotTcpConnectFinish(bool bsuccess);

private:
	void SetUI();
	// 设置用户登录时, 登录按钮和注册按钮是否可用
	bool EnableBtn(bool enabled);
	// 添加已存在的错误提示
	void AddTipErr(TipErr te, QString tips);
	// 删除已存在的错误提示
	void DelTipErr(TipErr te);
	// 根据请求的正确与否设置 error_label_ 的样式和文本
	void ShowErrorTip(QString str, bool b_ok);
	// 注册请求回调函数
	void InitHttpHandler();

private:
	// 存储相应的错误提示
	QMap<TipErr, QString> tip_errs_;
	// <req_id, 请求处理函数>
	QMap<ReqId, std::function<void(const QJsonObject &)>> handlers_;
	QString uid_;// 用户uid
    QString token_;// 用户token

private:
	// 错误提示
	QLabel *error_label_;
	QLabel *email_label_;			// 邮箱标签
	QLabel *password_label_;		// 密码标签
	QLineEdit *email_lineedit_;		// 邮箱输入框
	QLineEdit *password_lineedit_;	// 密码输入框
	QPushButton *login_button_;		// 登录按钮
	QPushButton *register_button_;	// 注册按钮
};

#endif	// LOGIN_DIALOG_H