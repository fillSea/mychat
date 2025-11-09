#include "search_user_item.h"

#include <QGridLayout>

SearchUserItem::SearchUserItem(QWidget* parent) : ListItemBase(parent) { SetUI(); }

SearchUserItem::~SearchUserItem() {}

void SearchUserItem::SetInfo(std::shared_ptr<SearchInfo> search_info) {
	search_info_ = search_info;
	QPixmap pixmap(search_info->icon_);
	// 设置图片自动缩放
	user_icon_->setPixmap(
		pixmap.scaled(user_icon_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	user_icon_->setScaledContents(true);

	name_label_->setText(search_info->name_);
}

std::shared_ptr<SearchInfo> SearchUserItem::GetInfo() { return search_info_; }

void SearchUserItem::SetUI() {
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