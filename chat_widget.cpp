#include "chat_widget.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

#include "UI/bubble/picture_bubble.h"
#include "UI/bubble/text_bubble.h"
#include "chat_item_base.h"
#include "chat_message_edit_widget.h"
#include "net/tcp_mgr.h"
#include "user_mgr.h"

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent) {
	SetUI();
	connect(message_edit_widget_, &ChatMessageEditWidget::SigSendMsg, this,
			&ChatWidget::SlotSendMsg);
	// // 设置按钮样式
	// ui->receive_btn->SetState("normal", "hover", "press");
	// ui->send_btn->SetState("normal", "hover", "press");

	// // 设置图标样式
	// ui->emo_lb->SetState("normal", "hover", "press", "normal", "hover", "press");
	// ui->file_lb->SetState("normal", "hover", "press", "normal", "hover", "press");
}

void ChatWidget::SetUserInfo(std::shared_ptr<UserInfo> user_info) {
	user_info_ = user_info;
	// 设置ui界面
	session_title_label_->setText(user_info_->name_);
	// message_show_widget_->removeAllItem();
	// for(auto & msg : user_info_->chat_msgs_){
	//     AppendChatMsg(msg);
	// }
}

void ChatWidget::AppendChatMsg(std::shared_ptr<TextChatData> msg) {
	auto self_info = UserMgr::GetInstance()->GetUserInfo();
	ChatRole role;
	if (msg->from_uid_ == self_info->uid_) {
		role = ChatRole::Self;
		ChatItemBase* pChatItem = new ChatItemBase(role);

		pChatItem->setUserName(self_info->name_);
		pChatItem->setUserIcon(QPixmap(self_info->icon_));
		QWidget* pBubble = nullptr;
		pBubble = new TextBubble(role, msg->msg_content_);
		pChatItem->setWidget(pBubble);
		message_show_widget_->addMessageWidget(pChatItem);
	} else {
		role = ChatRole::Other;
		ChatItemBase* pChatItem = new ChatItemBase(role);
		auto friend_info = UserMgr::GetInstance()->GetFriendById(msg->from_uid_);
		if (friend_info == nullptr) {
			return;
		}
		pChatItem->setUserName(friend_info->name_);
		pChatItem->setUserIcon(QPixmap(friend_info->icon_));
		QWidget* pBubble = nullptr;
		pBubble = new TextBubble(role, msg->msg_content_);
		pChatItem->setWidget(pBubble);
		message_show_widget_->addMessageWidget(pChatItem);
	}
}

ChatWidget::~ChatWidget() {}

void ChatWidget::SlotSendMsg() {
	if (user_info_ == nullptr) {
		qDebug() << "friend_info is empty";
		return;
	}

	auto user_info = UserMgr::GetInstance()->GetUserInfo();
	// auto pTextEdit = chatEditWidget_->GetTextEdit();
	ChatRole role = ChatRole::Self;
	QString userName = user_info_->name_;
	QString userIcon = user_info_->icon_;

	const QVector<MsgInfo>& msgList = message_edit_widget_->GetMsgList();
	QJsonObject textObj;
	QJsonArray textArray;
	int txt_size = 0;

	for (int i = 0; i < msgList.size(); ++i) {
		// 消息内容长度不合规就跳过
		if (msgList[i].content.length() > 1024) {
			continue;
		}
		QString type = msgList[i].msg_flag;
		MessageItem* pChatItem = new MessageItem(role);
		pChatItem->SetUserName(userName);
		pChatItem->SetUserIcon(QPixmap(userIcon));
		QWidget* pBubble = nullptr;
		if (type == "text") {
			// 生成唯一id
			QUuid uuid = QUuid::createUuid();
			// 转为字符串
			QString uuidString = uuid.toString();

			pBubble = new TextBubble(role, msgList[i].content);

			// 若累积要发送的数据超过1024字节，就发送一次
			if (txt_size + msgList[i].content.length() > 1024) {
				textObj["from_uid"] = user_info->uid_;
				textObj["to_uid"] = user_info_->uid_;
				textObj["text_array"] = textArray;
				QJsonDocument doc(textObj);
				QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
				// 发送并清空之前累计的文本列表
				txt_size = 0;
				textArray = QJsonArray();
				textObj = QJsonObject();
				// 发送tcp请求给chat server
				emit TcpMgr::GetInstance()
					-> SigSendData(ReqId::TEXT_CHAT_MSG_REQ, jsonData);
			}

			// 将bubble和uid绑定，以后可以等网络返回消息后设置是否送达
			//_bubble_map[uuidString] = pBubble;
			txt_size += msgList[i].content.length();
			QJsonObject obj;
			QByteArray utf8Message = msgList[i].content.toUtf8();
			obj["content"] = QString::fromUtf8(utf8Message);
			obj["unique_id"] = uuidString;
			textArray.append(obj);
			auto txt_msg = std::make_shared<TextChatData>(
				uuidString, obj["content"].toString(), user_info->uid_, user_info_->uid_);
			// emit SigAppendSendChatMsg(txt_msg);

		} else if (type == "image") {
			pBubble = new PictureBubble(QPixmap(msgList[i].content), role);
		} else if (type == "file") {
		}
		// 发送消息
		if (pBubble != nullptr) {
			pChatItem->SetWidget(pBubble);
			message_show_widget_->addMessageWidget(pChatItem);
		}
	}

	qDebug() << "textArray is " << textArray;
	// 发送给服务器
	textObj["text_array"] = textArray;
	textObj["from_uid"] = user_info->uid_;
	textObj["to_uid"] = user_info_->uid_;
	QJsonDocument doc(textObj);
	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
	// 发送并清空之前累计的文本列表
	txt_size = 0;
	textArray = QJsonArray();
	textObj = QJsonObject();
	// 发送tcp请求给chat server
	emit TcpMgr::GetInstance() -> SigSendData(ReqId::TEXT_CHAT_MSG_REQ, jsonData);
}

void ChatWidget::SetUI() {
	// 右侧窗口布局管理器
	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->setSpacing(0);
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->setAlignment(Qt::AlignTop);
	setLayout(vlayout);

	// 上方标题栏
	QWidget* title_widget = new QWidget();
	title_widget->setFixedHeight(60);
	title_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	title_widget->setObjectName("title_widget");
	title_widget->setStyleSheet(
		"#title_widget {"
		"    border-bottom: 1px solid rgb(225, 228, 235);"
		"    background-color: rgb(248, 249, 252);"
		"}");
	vlayout->addWidget(title_widget);

	// 标题栏内部布局
	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->setSpacing(12);
	hlayout->setContentsMargins(20, 0, 20, 0);
	title_widget->setLayout(hlayout);
	// 会话标题标签
	session_title_label_ = new QLabel();
	session_title_label_->setStyleSheet(
		"QLabel {"
		"    border-bottom: 1px solid rgb(225, 228, 235);"
		"    background-color: rgb(248, 249, 252);"
		"    font-size: 18px;"
		"    font-weight: 600;"
		"    color: rgb(60, 60, 60);"
		"    padding: 5px 0;"
		"}");
	hlayout->addWidget(session_title_label_, 1);

	// 额外功能按钮
	extra_btn_ = new QPushButton();
	extra_btn_->setFixedSize(36, 36);
	extra_btn_->setIconSize(QSize(24, 24));
	extra_btn_->setIcon(QIcon("../res/more.png"));
	extra_btn_->setCursor(Qt::PointingHandCursor);
	extra_btn_->setStyleSheet(
		"QPushButton {"
		"    border: none;"
		"    border-radius: 18px;"
		"    background-color: transparent;"
		"}"
		"QPushButton:hover {"
		"    background-color: rgba(200, 210, 230, 50);"
		"}"
		"QPushButton:pressed {"
		"    background-color: rgba(180, 190, 210, 80);"
		"}");
	hlayout->addWidget(extra_btn_, 0, Qt::AlignRight);

	// 消息展示区
	message_show_widget_ = new ChatMessageShowWidget();
	vlayout->addWidget(message_show_widget_, 1);

	// 消息编辑区
	message_edit_widget_ = new ChatMessageEditWidget();
	vlayout->addWidget(message_edit_widget_, 0, Qt::AlignBottom);

	// #if TEST_UI
	session_title_label_->setText("测试会话标题");
	// #endif
}
