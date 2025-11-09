#include "group_tip_item.h"

#include <QHBoxLayout>

GroupTipItem::GroupTipItem(QWidget* parent) : ListItemBase(parent) {
	SetItemType(ListItemType::GROUP_TIP_ITEM);
	SetUI();
}

QSize GroupTipItem::sizeHint() const {
	return QSize(250, 25);	// 返回自定义的尺寸
}

void GroupTipItem::SetGroupTip(QString str) { tip_label_->setText(str); }

void GroupTipItem::SetUI() {
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	tip_label_ = new QLabel(this);
	tip_label_->setAlignment(Qt::AlignCenter);
	tip_label_->setStyleSheet("font-size: 14px; color: #888888;");
	layout->addWidget(tip_label_);
}