#ifndef SELF_MESSAGE_WIDGET_H
#define SELF_MESSAGE_WIDGET_H
#include "message_item.h"

// 自消息项（靠右对齐）
class SelfMessageWidget : public MessageItem {
    Q_OBJECT
public:
    explicit SelfMessageWidget(QWidget *parent = nullptr);
    ~SelfMessageWidget();
    // 设置文字消息
    void setText(const QString &text) {
        QLabel *text_label = new QLabel(text);
        text_label->setStyleSheet("color: white; font-size: 14px; word-wrap: break-word;");
        text_label->setMaximumWidth(250);
        bubble_layout_->addWidget(text_label);
    }
private:
    void SetUI();
private:
    QWidget *bubble_widget_;      // 消息气泡
    QVBoxLayout *bubble_layout_;  // 气泡布局
};



#endif // SELF_MESSAGE_WIDGET_H
