#ifndef CHAT_WIDGET_H
#define CHAT_WIDGET_H
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "chat_message_edit_widget.h"
#include "chat_message_show_widget.h"
#include "user_data.h"


// 右侧聊天窗口
class ChatWidget : public QWidget {
	Q_OBJECT
public:
	explicit ChatWidget(QWidget* parent = nullptr);
	~ChatWidget();
	void SetUserInfo(std::shared_ptr<UserInfo> user_info);
	// 添加聊天消息
	void AppendChatMsg(std::shared_ptr<TextChatData> msg);

protected:
	// void paintEvent(QPaintEvent *event);
private slots:
	void SlotSendMsg();

	// void on_receive_btn_clicked();
private:
	// void clearItems();
	void SetUI();

private:
	std::shared_ptr<UserInfo> user_info_;
	// QMap<QString, QWidget *> _bubble_map;
	QLabel* session_title_label_;				  // 显示会话标题
	QPushButton* extra_btn_;					  // 显示会话详情按钮
	ChatMessageShowWidget* message_show_widget_;  // 消息展示区
	ChatMessageEditWidget* message_edit_widget_;  // 消息编辑区
signals:
	// void sig_append_send_chat_msg(std::shared_ptr<TextChatData> msg);
};

#endif	// CHAT_WIDGET_H