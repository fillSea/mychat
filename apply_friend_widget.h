#ifndef APPLY_FRIEND_WIDGET_H
#define APPLY_FRIEND_WIDGET_H
#include <QWidget>
#include <QLabel>
#include "apply_friend_list.h"
#include "apply_friend_item.h"

class ApplyFriendWidget : public QWidget{
    Q_OBJECT
public:
    explicit ApplyFriendWidget(QWidget *parent = nullptr);
private:
    void SetUI();
    void AddApplyList();
private slots:
    void SlotAuthRsp(std::shared_ptr<AuthRsp> auth_rsp);
private:
    // 未认证好友申请项
    std::unordered_map<QString, ApplyFriendItem*> unauth_items_;
private:
    QLabel* apply_friend_label_;//提示标签
    ApplyFriendList* apply_friend_list_;//好友申请列表
};


#endif//APPLY_FRIEND_WIDGET_H