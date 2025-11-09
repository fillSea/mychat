#ifndef PICTURE_BUBBLE_H
#define PICTURE_BUBBLE_H
#include "bubble_frame.h"

// 图片聊天气泡
class PictureBubble : public BubbleFrame {
	Q_OBJECT
public:
	PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent = nullptr);
private:
    void SetUI(const QPixmap &picture);
};

#endif	// PICTURE_BUBBLE_H