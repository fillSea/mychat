#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <qtimer.h>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>

#include "utils/clicked_label.h"
#include "const.h"
#include "timer_btn.h"


class RegisterDialog : public QDialog {
	Q_OBJECT
public:
	explicit RegisterDialog(QWidget *parent = nullptr);
signals:
	// 取消按钮点击, 返回登录界面
	void SigSwitchLogin();
private slots:
    // 注册用户功能
    void SlotRegisterUser();
	// 验证输入是否正确
	// 验证用户名
	bool SlotCheckUsername();
	// 验证密码
	bool SlotCheckPassword();
	// 验证重复密码
	bool SlotCheckConfirmPassword();
	// 验证邮箱是否正确
	bool SlotCheckEmail();
    // 验证验证码
    bool SlotCheckVerifyCode();
	// 注册模块完成槽函数
	void SlotRegModFinish(ReqId id, QString res, ErrorCodes err);
    // 获取验证码
    void SlotGetVerifycode();

private:
	// 注册请求回调函数
	void InitHttpHandler();
	// 根据请求的正确与否设置 error_label_ 的样式和文本
	void ShowErrorTip(QString str, bool b_ok);
	// 添加已存在的错误提示
	void AddTipErr(TipErr te, QString tips);
	// 删除已存在的错误提示
	void DelTipErr(TipErr te);
	void SetUI();		// 设置 UI 界面
	void SetQssName();	// 设置 qss 样式名称
	// 切换到注册成功提示页面
	void ChangeTipPage();
private:
	// <req_id, 请求处理函数>
	QMap<ReqId, std::function<void(const QJsonObject &)>> handlers_;
	// 存储相应的错误提示
	QMap<TipErr, QString> tip_errs_;
	
	QTimer * countdown_timer_; // 注册成功提示页面倒计时定时器
	int countdown_;
private:
	// 1-注册界面 2-注册成功提示页面
	QStackedWidget *stacked_widget_; // 用于切换注册表单和成功提示页面
	QWidget *register_page_; // 注册表单页面
	
	// 错误提示
	QLabel *error_label_;
	// 用户名、邮箱、密码、重复密码、验证码、注册按钮
	QLabel *username_label_;
	QLineEdit *username_lineedit_;
	
	QLabel *email_label_;
	QLineEdit *email_lineedit_;
	
	QLabel *password_label_;
	QLineEdit *password_lineedit_;
	
	QLabel *confirm_password_label_;
	QLineEdit *confirm_password_lineedit_;

	// 密码后面的显示/隐藏标签
	ClickedLabel *password_show_label_;
	ClickedLabel *confirm_password_show_label_;
	
	QLabel *verify_code_label_;
	QLineEdit *verify_code_lineedit_;
	
	// 获取验证码按钮
	TimerBtn *get_verify_code_button_;
	
	QPushButton *register_button_;
	// 取消按钮, 点击后切换回登录界面
	QPushButton *cancel_button_;
	
	// 注册成功提示界面
	QWidget *success_page_; // 成功提示页面
	QLabel *success_label_;// 成功提示文本
};

#endif