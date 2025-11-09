#ifndef BUBBLE_FRAME_H
#define BUBBLE_FRAME_H
#include <QFrame>
#include <QHBoxLayout>

#include "../../const.h"

// 气泡框基类
class BubbleFrame : public QFrame {
	Q_OBJECT
public:
	BubbleFrame(ChatRole role, QWidget *parent = nullptr);
	// void SetMargin(int margin);
	// 设置消息气泡的内容
	void SetWidget(QWidget *w);

protected:
	// 绘制事件, 绘制气泡框
	void paintEvent(QPaintEvent *e);
private:
	void SetUI();
private:
	QHBoxLayout *layout_;//水平布局
	ChatRole role_;//角色
	int margin_;//边距
};

#endif	// BUBBLE_FRAME_H
