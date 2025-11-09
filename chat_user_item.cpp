#include "chat_user_item.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

ChatUserItem::ChatUserItem(QWidget* parent) : ListItemBase(parent) {
	SetUI();
	SetItemType(ListItemType::CHAT_USER_ITEM);
	// 提升红点到顶部
	red_point_->raise();
	ShowRedPoint(false);
}

ChatUserItem::~ChatUserItem() {}

void ChatUserItem::UpdateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs) {
	QString last_msg = "";
	for (auto& msg : msgs) {
		last_msg = msg->msg_content_;
		user_info_->chat_msgs_.push_back(msg);
	}

	user_info_->last_msg_ = last_msg;
	message_label_->setText(user_info_->last_msg_);
}

// 重写sizeHint()方法，返回自定义的尺寸
QSize ChatUserItem::sizeHint() const {
	return QSize(250, 70);	// 返回自定义的尺寸
}

void ChatUserItem::SetInfo(std::shared_ptr<UserInfo> user_info) {
	user_info_ = user_info;
	// 加载图片
	QPixmap pixmap(user_info_->icon_);
	// 设置图片自动缩放
	user_icon_->setPixmap(
		pixmap.scaled(user_icon_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	user_icon_->setScaledContents(true);

	name_label_->setText(user_info_->name_);
	message_label_->setText(user_info_->last_msg_);
	time_label_->setText("10:10");
	// ShowRedPoint(true);
}

void ChatUserItem::SetInfo(std::shared_ptr<FriendInfo> friend_info) {
	user_info_ = std::make_shared<UserInfo>(friend_info->uid_, friend_info->name_, friend_info->back_name_, friend_info->icon_, friend_info->gender_);
	// 加载图片
	QPixmap pixmap(user_info_->icon_);

	// 设置图片自动缩放
	user_icon_->setPixmap(pixmap.scaled(user_icon_->size(), Qt::KeepAspectRatio,
										 Qt::SmoothTransformation));
	user_icon_->setScaledContents(true);

	name_label_->setText(user_info_->name_);
	message_label_->setText(user_info_->last_msg_);
}

// 显示红点
void ChatUserItem::ShowRedPoint(bool bshow) {
	if (bshow) {
		red_point_->show();
	} else {
		red_point_->hide();
	}
}

std::shared_ptr<UserInfo> ChatUserItem::GetUserInfo() { return user_info_; }

void ChatUserItem::SetUI() {
	setFixedHeight(70);
	setStyleSheet("QWidget { border-radius: 4px; }");

	// 创建网格布局管理器
	QGridLayout* layout = new QGridLayout();
	layout->setContentsMargins(15, 5, 10, 5);
	layout->setHorizontalSpacing(8);
	layout->setVerticalSpacing(0);
	setLayout(layout);

	// 创建头像
	user_icon_ = new QLabel();
	user_icon_->setFixedSize(46, 46);
	user_icon_->setStyleSheet(
		"QLabel {"
		"border: 1px solid rgba(200, 205, 220, 0.8);"
		"border-radius: 5px;"
		"background-color: transparent;"
		"}");

	// 创建红点
	red_point_ = new QLabel();
	red_point_->setFixedSize(10, 10);
	red_point_->setStyleSheet(
		"QLabel {"
		"border-radius: 5px;"
		"background-color: rgb(255, 0, 0);"
		"}");
	red_point_->setParent(user_icon_);	// 红点作为头像的子控件
	red_point_->move(36, 0);			// 右上角定位（46-10=36）
	red_point_->hide();

	// 创建名字
	name_label_ = new QLabel();
	// nameLabel->setText(name);
	name_label_->setStyleSheet(
		"QLabel {"
		"font-size: 15px;"
		"font-weight: 600;"
		"color: rgb(50, 50, 50);"
		"}");
	name_label_->setFixedHeight(28);

	// 创建消息预览的label
	message_label_ = new QLabel();
	// messageLabel->setText(text);
	message_label_->setStyleSheet(
		"QLabel {"
		"font-size: 13px;"
		"color: rgb(150, 150, 150);"
		"}");
	message_label_->setFixedHeight(24);
	// message_label_->setElideMode(Qt::ElideRight);  // 文字过长显示省略号
	message_label_->setTextInteractionFlags(Qt::TextSelectableByMouse);	 // 使文本可选
	message_label_->setWordWrap(false);									 // 禁止自动换行
	// 设置完整文本作为tooltip，这样用户可以看到完整内容
	message_label_->setToolTip(message_label_->text());

	// 创建时间的label
	time_label_ = new QLabel();
	// timeLabel->setText(time);
	time_label_->setStyleSheet(
		"QLabel {"
		"font-size: 13px;"
		"color: rgb(150, 150, 150);"
		"}");
	time_label_->setFixedHeight(24);

	layout->addWidget(user_icon_, 0, 0, 2, 2);
	// layout->addWidget(red_point_, 0, 1, 1, 1);
	layout->addWidget(name_label_, 0, 2, 1, 1);
	layout->addWidget(message_label_, 1, 2, 1, 1);
	layout->addWidget(time_label_, 0, 3, 1, 1);
	layout->setColumnStretch(2, 1);	 // 第2列（文字区）拉伸占满空间
	// layout->setColumnMinimumWidth(2, 60);  // 确保时间列至少有60像素宽度
}