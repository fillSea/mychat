#ifndef CHAT_MESSAGE_EDIT_WIDGET_H
#define CHAT_MESSAGE_EDIT_WIDGET_H
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>
#include "message_text_edit.h"


class ChatMessageEditWidget : public QWidget {
	Q_OBJECT
public:
	explicit ChatMessageEditWidget(QWidget* parent = nullptr);
	// 获取消息列表
	QVector<MsgInfo> GetMsgList() { return text_edit_->GetMsgList(); }
private:
    void SetUI();
signals:
	void SigSendMsg();
private slots:
private:
	QPushButton* send_image_btn_;//发送图片按钮
	QPushButton* send_file_btn_;//发送文件按钮
	QPushButton* send_speech_btn_;//发送语音按钮
	// QPushButton* show_history_btn;//显示历史消息按钮
	MessageTextEdit* text_edit_;//文本编辑框
	QPushButton* send_btn_;//发送文本按钮
	// QLabel* tip_label_;//提示标签
};

#endif	// CHAT_MESSAGE_EDIT_WIDGET_H