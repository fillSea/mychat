#ifndef CHATUSERITEM_H
#define CHATUSERITEM_H

#include <QWidget>

#include "user_data.h"
#include "utils/list_item_base.h"
#include <QPushButton>
#include <QLabel>


class ChatUserItem : public ListItemBase {
	Q_OBJECT

public:
	explicit ChatUserItem(QWidget *parent = nullptr);
	~ChatUserItem();
	QSize sizeHint() const override;
    // 设置用户信息
	void SetInfo(std::shared_ptr<UserInfo> user_info);
	void SetInfo(std::shared_ptr<FriendInfo> friend_info);
    // 显示红点
	void ShowRedPoint(bool bshow);
	std::shared_ptr<UserInfo> GetUserInfo();
	void UpdateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);
private:
    void SetUI();
private:
	std::shared_ptr<UserInfo> user_info_;  // 用户信息
private:
    // Widget
    QLabel* user_icon_;// 头像
    QLabel* red_point_;// 红点, 用于提示有新消息
    // Widget
    QLabel* name_label_;// 用户名
    QLabel* message_label_;// 预览消息
    // Widget
    QLabel* time_label_;// 时间
};

#endif	// CHATUSERITEM_H
