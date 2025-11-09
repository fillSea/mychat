#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>

// 用户会话列表
class ChatUserList: public QListWidget{
    Q_OBJECT
public:
    ChatUserList(QWidget *parent = nullptr);

protected:
    // 事件过滤器
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void SlotItemClicked(QListWidgetItem *item);
signals:
    void SigLoadingChatUser();
private:
    bool load_pending_;// 是否正在加载中
};

#endif // CHATUSERLIST_H
