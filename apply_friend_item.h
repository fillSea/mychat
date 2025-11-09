#ifndef APPLY_FRIEND_ITEM_H
#define APPLY_FRIEND_ITEM_H
#include <QLabel>

#include "user_data.h"
#include "utils/clicked_btn.h"
#include "utils/list_item_base.h"


// TODO:拒绝添加好友
//TODO: UI样式
class ApplyFriendItem : public ListItemBase {
	Q_OBJECT
public:
	explicit ApplyFriendItem(QWidget* parent = nullptr);
	void SetApplyInfo(std::shared_ptr<ApplyInfo> apply_info);
	void ShowAddBtn(bool bshow);
	QSize sizeHint() const override;
	QString GetUid();

private:
	void SetUI();
	void SetQssName();
signals:
	void SigAuthFriend(std::shared_ptr<ApplyInfo> apply_info);
private:
	bool added_;
	std::shared_ptr<ApplyInfo> apply_info_;

private:
	QLabel* icon_label_;		 // 头像
	QLabel* name_label_;		 // 用户名
	QLabel* hello_label_;		 // 打招呼
	ClickedBtn* add_btn_;		 // 添加好友按钮
	QLabel* already_add_label_;	 // 已添加好友
};

#endif	// APPLY_FRIEND_ITEM_H