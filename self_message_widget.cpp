#include "self_message_widget.h"

SelfMessageWidget::SelfMessageWidget(QWidget *parent) : MessageItem(parent) { SetUI(); }

SelfMessageWidget::~SelfMessageWidget() {}

void SelfMessageWidget::SetUI() {
	// 内容布局（右对齐：内容 + 头像）
	QHBoxLayout *content_layout = new QHBoxLayout(content_widget_);
	content_layout->setContentsMargins(0, 0, 0, 0);
	content_layout->setSpacing(8);

	// 消息气泡（蓝色背景）
	bubble_widget_ = new QWidget;
	bubble_widget_->setStyleSheet(
		"background-color: #0078d7;"
		"border-radius: 12px;"
		"padding: 8px 12px;");
	bubble_layout_ = new QVBoxLayout(bubble_widget_);
	bubble_layout_->setContentsMargins(0, 0, 0, 0);

	// 组装内容布局：气泡 + 头像
	content_layout->addWidget(bubble_widget_);
	content_layout->addWidget(icon_label_);

	main_layout_->addWidget(content_widget_, 0, Qt::AlignRight);
}
