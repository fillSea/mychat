#ifndef TIMER_BTN_H
#define TIMER_BTN_H
#include <QPushButton>

// 倒计时按钮
class TimerBtn : public QPushButton{
public:
    TimerBtn(QWidget* parent = nullptr);
    ~TimerBtn();

    // 重写mouseReleaseEvent
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
private:
    QTimer *timer_;
    int counter_;
};

#endif	// TIMER_BTN_H
