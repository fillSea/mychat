#include "apply_friend_item.h"

#include <qboxlayout.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "utils/clicked_btn.h"
#include "utils/util.h"


ApplyFriendItem::ApplyFriendItem(QWidget* parent) : ListItemBase(parent), added_(false) {
	SetUI();

	SetItemType(ListItemType::APPLY_FRIEND_ITEM);
	add_btn_->SetState("normal", "hover", "press");
	// ShowAddBtn(true);
	// add_btn_->hide();
	connect(add_btn_, &ClickedBtn::clicked, [this]() {
		emit this->SigAuthFriend(apply_info_);
	});
}

void ApplyFriendItem::SetApplyInfo(std::shared_ptr<ApplyInfo> apply_info) {
	apply_info_ = apply_info;
	// 加载图片
	QPixmap pixmap(apply_info_->icon_);

	// 设置图片自动缩放
	icon_label_->setPixmap(pixmap.scaled(icon_label_->size(), Qt::KeepAspectRatio,
										 Qt::SmoothTransformation));
	icon_label_->setScaledContents(true);

	name_label_->setText(apply_info_->name_);
	hello_label_->setText(apply_info_->desc_);
}

void ApplyFriendItem::ShowAddBtn(bool bshow) {
	if (bshow) {
		add_btn_->show();
		already_add_label_->hide();
		added_ = false;
	} else {
		add_btn_->hide();
		already_add_label_->show();
		added_ = true;
	}
}

QSize ApplyFriendItem::sizeHint() const {
	return QSize(250, 80);	// 返回自定义的尺寸
}

QString ApplyFriendItem::GetUid() { return apply_info_->uid_; }

void ApplyFriendItem::SetQssName() { add_btn_->setObjectName("add_btn"); }

void ApplyFriendItem::SetUI() {
	QHBoxLayout* layout = new QHBoxLayout(this);
	// layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	icon_label_ = new QLabel(this);
	icon_label_->setFixedSize(40, 40);
	layout->addWidget(icon_label_);

	QHBoxLayout* hbox_layout = new QHBoxLayout();

	QVBoxLayout* vbox_layout = new QVBoxLayout();
	name_label_ = new QLabel(this);
	name_label_->setText("用户名");
	vbox_layout->addWidget(name_label_);
	hello_label_ = new QLabel(this);
	hello_label_->setText("你好");
	vbox_layout->addWidget(hello_label_);

	hbox_layout->addLayout(vbox_layout);
	layout->addLayout(hbox_layout);
    layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	add_btn_ = new ClickedBtn(this);
	add_btn_->setText("添加");
	layout->addWidget(add_btn_);

	already_add_label_ = new QLabel(this);
	already_add_label_->setText("已添加");
	layout->addWidget(already_add_label_);

	layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	SetQssName();
	LoadStyleSheet(this, "../style/apply_friend_item.qss");
}
