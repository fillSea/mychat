#include "bubble_frame.h"

#include <QPainter>


constexpr int WIDTH_SANJIAO = 8;  // 三角宽

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent)
	: QFrame(parent), role_(role), margin_(3) {
	SetUI();
}

void BubbleFrame::SetUI() {
	layout_ = new QHBoxLayout();
	if (role_ == ChatRole::Self)
		layout_->setContentsMargins(margin_, margin_, WIDTH_SANJIAO + margin_, margin_);
	else
		layout_->setContentsMargins(WIDTH_SANJIAO + margin_, margin_, margin_, margin_);

	this->setLayout(layout_);
}

// 设置消息气泡的内容
void BubbleFrame::SetWidget(QWidget *w) {
	// 是否已有内容，已有内容则不添加
	if (layout_->count() > 0)
		return;
	else {
		// 没有内容则添加
		layout_->addWidget(w);
	}
}

// 绘制事件, 绘制气泡框
void BubbleFrame::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
    // 设置画笔为无画框
	painter.setPen(Qt::NoPen);

    if (role_ == ChatRole::Self){
        // 绿色气泡
        QColor bk_color(158, 234, 106);
		painter.setBrush(QBrush(bk_color));
		// 画气泡
		QRect bk_rect = QRect(0, 0, this->width() - WIDTH_SANJIAO, this->height());
		// 设置气泡圆角
        painter.drawRoundedRect(bk_rect, 5, 5);
		// 画三角, 向右指向
		QPointF points[3] = {
			QPointF(bk_rect.x() + bk_rect.width(), 12),
			QPointF(bk_rect.x() + bk_rect.width(), 12 + WIDTH_SANJIAO + 2),
			QPointF(bk_rect.x() + bk_rect.width() + WIDTH_SANJIAO,
					10 + WIDTH_SANJIAO - WIDTH_SANJIAO / 2.0),
		};
		painter.drawPolygon(points, 3);
    } else {
        // 画气泡
		QColor bk_color(Qt::white);
		painter.setBrush(QBrush(bk_color));
		QRect bk_rect =
			QRect(WIDTH_SANJIAO, 0, this->width() - WIDTH_SANJIAO, this->height());
		painter.drawRoundedRect(bk_rect, 5, 5);
		// 画小三角
		QPointF points[3] = {
			QPointF(bk_rect.x(), 12),
			QPointF(bk_rect.x(), 10 + WIDTH_SANJIAO + 2),
			QPointF(bk_rect.x() - WIDTH_SANJIAO, 10 + WIDTH_SANJIAO - WIDTH_SANJIAO / 2.0),
		};
        // 绘制三角形
		painter.drawPolygon(points, 3);
    }

	return QFrame::paintEvent(e);
}