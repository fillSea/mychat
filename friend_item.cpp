#include "friend_item.h"

#include <QGridLayout>


FriendItem::FriendItem(QWidget* parent) : ListItemBase(parent) {
	SetUI();
	SetItemType(ListItemType::CONTACT_USER_ITEM);
	// 提升红点到顶部
	red_point_->raise();
	ShowRedPoint(false);
}

// 重写sizeHint()方法，返回自定义的尺寸
QSize FriendItem::sizeHint() const {
	return QSize(250, 70);	// 返回自定义的尺寸
}

void FriendItem::SetUserInfo(std::shared_ptr<UserInfo> user_info) {
	user_info_ = user_info;
	// 加载图片
	QPixmap pixmap(user_info_->icon_);
	// 设置图片自动缩放
	user_icon_->setPixmap(
		pixmap.scaled(user_icon_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	user_icon_->setScaledContents(true);

	name_label_->setText(user_info_->name_ + "(" + user_info_->back_name_ + ")");
}

void FriendItem::SetInfo(QString uid, QString name, QString icon) {
	user_info_ = std::make_shared<UserInfo>(uid, name, "", icon, 0);
	// 加载图片
	QPixmap pixmap(user_info_->icon_);

	// 设置图片自动缩放
	user_icon_->setPixmap(
		pixmap.scaled(user_icon_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	user_icon_->setScaledContents(true);

	name_label_->setText(user_info_->name_);
}

// 显示红点
void FriendItem::ShowRedPoint(bool bshow) {
	if (bshow) {
		red_point_->show();
	} else {
		red_point_->hide();
	}
}

std::shared_ptr<UserInfo> FriendItem::GetUserInfo() { return user_info_; }

void FriendItem::SetUI() {
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

	layout->addWidget(user_icon_, 0, 0, 2, 2);
	layout->addWidget(name_label_, 0, 2, 1, 1);
}