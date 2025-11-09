#include "clicked_btn.h"

#include <QKeyEvent>
#include <QVariant>

#include "util.h"


ClickedBtn::ClickedBtn(QWidget *parent) : QPushButton(parent) {
	setCursor(Qt::PointingHandCursor);	// 设置光标为小手
	setFocusPolicy(Qt::NoFocus);
}

ClickedBtn::~ClickedBtn() {}

void ClickedBtn::SetState(QString normal, QString hover, QString press) {
	hover_ = hover;
	normal_ = normal;
	press_ = press;
	setProperty("state", normal);
	repolish(this);
	update();
}

void ClickedBtn::enterEvent(QEnterEvent *event) {
	setProperty("state", hover_);
	repolish(this);
	update();
	QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event) {
	setProperty("state", normal_);
	repolish(this);
	update();
	QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent *event) {
	setProperty("state", press_);
	repolish(this);
	update();
	QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *event) {
	setProperty("state", hover_);
	repolish(this);
	update();
	QPushButton::mouseReleaseEvent(event);
}
