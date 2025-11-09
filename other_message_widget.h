#ifndef OTHER_MESSAGE_WIDGET_H
#define OTHER_MESSAGE_WIDGET_H
#include "message_item.h"

class OtherMessageWidget : public MessageItem {
    Q_OBJECT
public:
    explicit OtherMessageWidget(QWidget *parent = nullptr);
    ~OtherMessageWidget();
private:
    void SetUI();
private:
    QWidget *bubble_widget_;      // 消息气泡
    QVBoxLayout *bubble_layout_;  // 气泡布局
};



#endif//OTHER_MESSAGE_WIDGET_H