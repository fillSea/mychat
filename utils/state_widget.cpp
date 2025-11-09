#include "state_widget.h"

#include <QPixmap>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

#include "util.h"

StateWidget::StateWidget(QWidget* parent)
	: QWidget(parent), cur_state_(ClickLbState::Normal) {
	setCursor(Qt::PointingHandCursor);
	// 添加红点
	AddRedPoint();
}

void StateWidget::AddRedPoint() {
	// 添加红点示意图
	red_point_ = new QLabel();
	red_point_->setObjectName("red_point");
	red_point_->setPixmap(QPixmap("../res/red_point.png"));
	QVBoxLayout* layout2 = new QVBoxLayout;
	red_point_->setAlignment(Qt::AlignCenter);
	layout2->addWidget(red_point_);
	layout2->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout2);
	red_point_->setVisible(false);
}

void StateWidget::ShowRedPoint(bool show) { red_point_->setVisible(true); }

void StateWidget::SetState(QString normal, QString hover, QString press, QString select,
						   QString select_hover, QString select_press) {
	normal_ = normal;
	hover_ = hover;
	press_ = press;

	select_ = select;
	select_hover_ = select_hover;
	select_press_ = select_press;

	setProperty("state", normal);
	repolish(this);
}

ClickLbState StateWidget::GetCurState() { return cur_state_; }

void StateWidget::ClearState() {
	cur_state_ = ClickLbState::Normal;
	setProperty("state", normal_);
	repolish(this);
	update();
}

void StateWidget::SetSelected(bool bselected) {
	if (bselected) {
		cur_state_ = ClickLbState::Selected;
		setProperty("state", select_);
		repolish(this);
		update();
		return;
	}

	cur_state_ = ClickLbState::Normal;
	setProperty("state", normal_);
	repolish(this);
	update();
	return;
}

void StateWidget::paintEvent(QPaintEvent* event) {
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	return;
}

// 处理鼠标点击事件
void StateWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		if (cur_state_ == ClickLbState::Selected) {
			emit clicked();
			//  调用基类的mousePressEvent以保证正常的事件处理
			QWidget::mousePressEvent(event);
			return;
		}

		if (cur_state_ == ClickLbState::Normal) {
			qDebug() << "PressEvent , change to selected press: " << select_press_;
			cur_state_ = ClickLbState::Selected;
			setProperty("state", select_press_);
			repolish(this);
			update();
		}

		return;
	}
	// 调用基类的mousePressEvent以保证正常的事件处理
	QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		if (cur_state_ == ClickLbState::Normal) {
			// qDebug()<<"ReleaseEvent , change to normal hover: "<< _normal_hover;
			setProperty("state", hover_);
			repolish(this);
			update();

		} else {
			// qDebug()<<"ReleaseEvent , change to select hover: "<< _selected_hover;
			setProperty("state", select_hover_);
			repolish(this);
			update();
		}
		emit clicked();
		return;
	}
	// 调用基类的mousePressEvent以保证正常的事件处理
	QWidget::mousePressEvent(event);
}

// 处理鼠标悬停进入事件
void StateWidget::enterEvent(QEnterEvent* event) {
	// 在这里处理鼠标悬停进入的逻辑
	if (cur_state_ == ClickLbState::Normal) {
		// qDebug()<<"enter , change to normal hover: "<< _normal_hover;
		setProperty("state", hover_);
		repolish(this);
		update();

	} else {
		// qDebug()<<"enter , change to selected hover: "<< _selected_hover;
		setProperty("state", select_hover_);
		repolish(this);
		update();
	}

	QWidget::enterEvent(event);
}

// 处理鼠标悬停离开事件
void StateWidget::leaveEvent(QEvent* event) {
	// 在这里处理鼠标悬停离开的逻辑
	if (cur_state_ == ClickLbState::Normal) {
		// qDebug()<<"leave , change to normal : "<< _normal;
		setProperty("state", normal_);
		repolish(this);
		update();

	} else {
		// qDebug()<<"leave , change to select normal : "<< _selected;
		setProperty("state", select_);
		repolish(this);
		update();
	}
	QWidget::leaveEvent(event);
}