#ifndef MESSAGE_ITEM_H
#define MESSAGE_ITEM_H
#include <qtmetamacros.h>

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "const.h"

// 消息项基类
class MessageItem : public QWidget {
	Q_OBJECT
public:
	explicit MessageItem(ChatRole role = ChatRole::Self, QWidget *parent = nullptr);
	~MessageItem();
    // 设置用户名
    void SetUserName(const QString &name);
    // 设置头像
    void SetUserIcon(const QPixmap &icon);
    // 设置消息气泡
    void SetWidget(QWidget *w);
private:
    void SetUI();
protected:
    ChatRole role_;// 聊天角色
    QLabel *name_label_;// 用户名标签
	QLabel *icon_label_;		// 头像标签
    QWidget *bubble_widget_;	// 消息气泡容器
};

#endif	// MESSAGE_ITEM_H