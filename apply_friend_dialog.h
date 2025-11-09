#ifndef APPLY_FRIEND_DIALOG_H
#define APPLY_FRIEND_DIALOG_H
#include <qlabel.h>

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <memory>

#include "user_data.h"


//  好友申请对话框
class ApplyFriendDialog : public QDialog {
	Q_OBJECT
public:
	explicit ApplyFriendDialog(QWidget* parent = nullptr);
	~ApplyFriendDialog();
	void SetInfo(std::shared_ptr<SearchInfo> info);

private:
	void SetUI();
private slots:
    // 好友申请按钮点击槽函数
	void SlotApplyFriend();

private:
	std::shared_ptr<SearchInfo> search_info_;  // 申请好友信息
private:
	QLabel* title_label_;  // 标题
	// QWidget* content_widget_;// 内容区域
	QLabel* apply_user_name_label_;			   // 申请好友用户名
	QLineEdit* apply_user_name_lineedit_;	   // 申请好友用户名输入框
	QLabel* apply_user_backname_label_;		   // 申请好友备注
	QLineEdit* apply_user_backname_lineedit_;  // 申请好友备注输入框
	// 招呼语
	QLabel* apply_user_hello_label_;		// 申请好友招呼语
	QLineEdit* apply_user_hello_lineedit_;	// 申请好友招呼语输入框
	// TODO: 添加标签
	QPushButton* apply_button_;	  // 申请按钮
	QPushButton* cancel_button_;  // 取消按钮
};

#endif	// APPLY_FRIEND_DIALOG_H