#ifndef FRIEND_ITEM_H
#define FRIEND_ITEM_H
#include <QLabel>
#include <QPushButton>

#include "user_data.h"
#include "utils/list_item_base.h"

class FriendItem : public ListItemBase {
	Q_OBJECT
public:
	explicit FriendItem(QWidget* parent = nullptr);
	// 设置用户信息
	void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void SetInfo(QString uid, QString name, QString icon);
	// 显示红点
	void ShowRedPoint(bool bshow);
	std::shared_ptr<UserInfo> GetUserInfo();
    QSize sizeHint() const override;

private:
	void SetUI();

private:
	std::shared_ptr<UserInfo> user_info_;  // 用户信息
private:
	// Widget
	QLabel* user_icon_;	 // 头像
	QLabel* red_point_;	 // 红点, 用于提示有新消息
	// Widget
	QLabel* name_label_;  // 用户名, username(back_name)
};

#endif	// FRIEND_ITEM_H