#ifndef APPLY_USER_LIST_H
#define APPLY_USER_LIST_H
#include <QListWidget>

// 好友申请列表
class ApplyFriendList : public QListWidget{
    Q_OBJECT
public:
    explicit ApplyFriendList(QWidget* parent = nullptr);
protected:
    // 事件过滤器
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void SigLoadingApplyUser();
private:
};


#endif//APPLY_USER_LIST_H