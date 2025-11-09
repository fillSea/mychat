#include "message_item.h"

#include <QGridLayout>

#include "const.h"

MessageItem::MessageItem(ChatRole role, QWidget* parent) : QWidget(parent), role_(role) {
	SetUI();
}

MessageItem::~MessageItem() {}

void MessageItem::SetUserName(const QString& name) { name_label_->setText(name); }

void MessageItem::SetUserIcon(const QPixmap& icon) {
	icon_label_->setPixmap(
		icon.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 设置消息气泡
void MessageItem::SetWidget(QWidget* w) {
	QGridLayout* pGLayout = (qobject_cast<QGridLayout*>)(this->layout());
	pGLayout->replaceWidget(bubble_widget_, w);
	delete bubble_widget_;
	bubble_widget_ = w;

}

void MessageItem::SetUI() {
	// 用户名标签
	name_label_ = new QLabel;
	name_label_->setStyleSheet(
		"font-size: 14px; font-family: \"Microsoft YaHei\"; color:rgb(153,153,153);");
	name_label_->setAlignment(Qt::AlignCenter);

	// 头像标签
	icon_label_ = new QLabel;
	icon_label_->setFixedSize(36, 36);
	// icon_label_->setStyleSheet("border-radius: 18px;");	 // 圆形头像
	icon_label_->setScaledContents(true);

	bubble_widget_ = new QWidget;

	QGridLayout* grid_layout = new QGridLayout();
	grid_layout->setVerticalSpacing(3);
	grid_layout->setHorizontalSpacing(3);
	grid_layout->setContentsMargins(3, 3, 3, 3);

	QSpacerItem* pSpacer =
		new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	if (role_ == ChatRole::Self) {
		name_label_->setContentsMargins(0, 0, 8, 0);
		name_label_->setAlignment(Qt::AlignRight);
		grid_layout->addWidget(name_label_, 0, 1, 1, 1);
		grid_layout->addWidget(icon_label_, 0, 2, 2, 1, Qt::AlignTop);
		grid_layout->addItem(pSpacer, 1, 0, 1, 1);
		grid_layout->addWidget(bubble_widget_, 1, 1, 1, 1);
		grid_layout->setColumnStretch(0, 2);
		grid_layout->setColumnStretch(1, 3);
	} else {
		name_label_->setContentsMargins(8, 0, 0, 0);
		name_label_->setAlignment(Qt::AlignLeft);
		grid_layout->addWidget(icon_label_, 0, 0, 2, 1, Qt::AlignTop);
		grid_layout->addWidget(name_label_, 0, 1, 1, 1);
		grid_layout->addWidget(bubble_widget_, 1, 1, 1, 1);
		grid_layout->addItem(pSpacer, 2, 2, 1, 1);
		grid_layout->setColumnStretch(1, 3);
		grid_layout->setColumnStretch(2, 2);
	}
	this->setLayout(grid_layout);
}