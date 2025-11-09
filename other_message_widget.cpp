#include "other_message_widget.h"

OtherMessageWidget::OtherMessageWidget(QWidget *parent) : MessageItem(parent) { SetUI(); }

OtherMessageWidget::~OtherMessageWidget() {}

void OtherMessageWidget::SetUI() {
    // 内容布局（左对齐：头像 + 内容）
        QHBoxLayout *content_layout = new QHBoxLayout(content_widget_);
        content_layout->setContentsMargins(0, 0, 0, 0);
        content_layout->setSpacing(8);

        // 消息气泡（灰色背景）
        bubble_widget_ = new QWidget;
        bubble_widget_->setStyleSheet(
            "background-color: #f0f0f0;"
            "border-radius: 12px;"
            "padding: 8px 12px;"
        );
        bubble_layout_ = new QVBoxLayout(bubble_widget_);
        bubble_layout_->setContentsMargins(0, 0, 0, 0);

        // 组装内容布局：头像 + 气泡
        content_layout->addWidget(icon_label_);
        content_layout->addWidget(bubble_widget_);

        main_layout_->addWidget(content_widget_, 0, Qt::AlignLeft);
}
