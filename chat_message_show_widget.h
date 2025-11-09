#ifndef CHAT_MESSAGE_SHOW_WIDGET_H
#define CHAT_MESSAGE_SHOW_WIDGET_H
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "self_message_widget.h"


// 聊天展示区域
class ChatMessageShowWidget : public QWidget {
	Q_OBJECT
public:
	explicit ChatMessageShowWidget(QWidget *parent = nullptr);
	~ChatMessageShowWidget();
	// 通用添加消息控件（自动滚动到底部）
	void addMessageWidget(QWidget *widget) {
		QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(scroll_area_->widget()->layout());
		qDebug() << "vl->count() is " << vl->count();
		vl->insertWidget(vl->count() - 1, widget);
		// message_layout_->insertWidget(message_layout_->count() - 1, widget); //
		// 插入到拉伸项之前 滚动到底部 QTimer::singleShot(0, this, [=]() {
		//     QScrollBar *scroll_bar = scroll_area_->verticalScrollBar();
		//     scroll_bar->setValue(scroll_bar->maximum());
		// });
	}
	// 添加自己的文字消息
	void addSelfTextMessage(const QString &text, const QString &time,
							const QPixmap &avatar) {
		SelfMessageWidget *msg_widget = new SelfMessageWidget;
		// msg_widget->setTime(time);
		msg_widget->SetUserIcon(avatar);
		msg_widget->setText(text);
		addMessageWidget(msg_widget);
	}

private:
	void SetUI();

private:
	QScrollArea *scroll_area_;	   // 滚动区域
	QWidget *message_container_;   // 消息容器
	// QVBoxLayout *message_layout_;  // 消息布局
};

#endif	// CHAT_MESSAGE_SHOW_WIDGET_H