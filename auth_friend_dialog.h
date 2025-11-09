#ifndef AUTH_FRIEND_DIALOG_H
#define AUTH_FRIEND_DIALOG_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "user_data.h"

//  好友认证对话框
class AuthFriendDialog : public QDialog {
	Q_OBJECT
public:
	explicit AuthFriendDialog(QWidget* parent = nullptr);
	// ~AuthFriendDialog();
	void SetApplyInfo(std::shared_ptr<ApplyInfo> info);

private:
	void SetUI();
private slots:
    // 好友认证按钮点击槽函数
	void SlotAuthFriend();

private:
	std::shared_ptr<ApplyInfo> apply_info_;  // 认证好友信息
private:
	QLabel* title_label_;  // 标题
	// QWidget* content_widget_;// 内容区域
	QLabel* auth_user_name_label_;			   // 认证好友用户名
	QLineEdit* auth_user_name_lineedit_;	   // 认证好友用户名输入框
	QLabel* auth_user_backname_label_;		   // 认证好友备注
	QLineEdit* auth_user_backname_lineedit_;  // 认证好友备注输入框
	// 招呼语
	QLabel* auth_user_hello_label_;		// 认证好友招呼语
	QLineEdit* auth_user_hello_lineedit_;	// 认证好友招呼语输入框
	// TODO: 添加标签
	QPushButton* auth_button_;	  // 认证按钮
	QPushButton* cancel_button_;  // 取消按钮
};

#endif//AUTH_FRIEND_DIALOG_H