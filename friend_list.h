#ifndef FRIEND_LIST_H
#define FRIEND_LIST_H
#include <QListWidget>
#include <memory>
#include "friend_item.h"
#include "user_data.h"

// 好友列表
class FriendList : public QListWidget {
	Q_OBJECT
public:
	explicit FriendList(QWidget *parent = nullptr);

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void SetUI();
    void AddFriendList();
private slots:
    void SlotItemClicked(QListWidgetItem* item);
    void SlotAuthRsp(std::shared_ptr<AuthRsp> auth_rsp);
signals:
	void SigLoadingFriend();
    void SigSwitchApplyFriendWidget();
    void SigSwitchChat(std::shared_ptr<UserInfo> user_info);

private:
    FriendItem * apply_item_;//申请好友项
    QListWidgetItem * group_item_;
};

#endif	// FRIEND_LIST_H