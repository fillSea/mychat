#include "chat_item_base.h"

#include <QGridLayout>


ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent)
	: QWidget(parent), role_(role) {
	SetUI();
}

void ChatItemBase::setUserName(const QString &name) { name_label_->setText(name); }

void ChatItemBase::setUserIcon(const QPixmap &icon) { icon_label_->setPixmap(icon); }

void ChatItemBase::setWidget(QWidget *w) {
	QGridLayout *pGLayout = (qobject_cast<QGridLayout *>)(this->layout());
	pGLayout->replaceWidget(bubble_, w);
	delete bubble_;
	bubble_ = w;
}

void ChatItemBase::SetUI() {
	name_label_ = new QLabel();
	name_label_->setObjectName("chat_user_name");
	QFont font("Microsoft YaHei");
	font.setPointSize(9);
	name_label_->setFont(font);
	name_label_->setFixedHeight(20);
	icon_label_ = new QLabel();
	icon_label_->setScaledContents(true);
	icon_label_->setFixedSize(42, 42);
	bubble_ = new QWidget();
	QGridLayout *pGLayout = new QGridLayout();
	pGLayout->setVerticalSpacing(3);
	pGLayout->setHorizontalSpacing(3);
	pGLayout->setContentsMargins(3, 3, 3, 3);
	QSpacerItem *pSpacer =
		new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	if (role_ == ChatRole::Self) {
		name_label_->setContentsMargins(0, 0, 8, 0);
		name_label_->setAlignment(Qt::AlignRight);
		pGLayout->addWidget(name_label_, 0, 1, 1, 1);
		pGLayout->addWidget(icon_label_, 0, 2, 2, 1, Qt::AlignTop);
		pGLayout->addItem(pSpacer, 1, 0, 1, 1);
		pGLayout->addWidget(bubble_, 1, 1, 1, 1);
		pGLayout->setColumnStretch(0, 2);
		pGLayout->setColumnStretch(1, 3);
	} else {
		name_label_->setContentsMargins(8, 0, 0, 0);
		name_label_->setAlignment(Qt::AlignLeft);
		pGLayout->addWidget(icon_label_, 0, 0, 2, 1, Qt::AlignTop);
		pGLayout->addWidget(name_label_, 0, 1, 1, 1);
		pGLayout->addWidget(bubble_, 1, 1, 1, 1);
		pGLayout->addItem(pSpacer, 2, 2, 1, 1);
		pGLayout->setColumnStretch(1, 3);
		pGLayout->setColumnStretch(2, 2);
	}
	this->setLayout(pGLayout);
}