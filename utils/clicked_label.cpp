#include "clicked_label.h"
#include <qevent.h>

#include <QMouseEvent>
#include <QEvent>

#include "util.h"

ClickedLabel::ClickedLabel(QWidget* parent)
	: QLabel(parent), curstate_(ClickLbState::Normal) {
	// 将鼠标指针设置为手形指针
	setCursor(Qt::PointingHandCursor);
}

// 处理鼠标点击事件
/**
 * @brief 处理鼠标按下事件，实现标签的点击交互和状态切换
 * @param event 鼠标事件对象，包含事件的详细信息
 * 
 * 此函数重写了 QLabel 的 mousePressEvent 方法，专门处理鼠标左键点击
 * 实现了标签在 Normal 和 Selected 两种状态之间的切换，并提供相应的视觉反馈
 */
void ClickedLabel::mousePressEvent(QMouseEvent* event) {
    // 仅处理鼠标左键点击事件
    if (event->button() == Qt::LeftButton) {
        // 根据当前状态进行状态切换
        if (curstate_ == ClickLbState::Normal) {
            // 从正常状态切换到选中状态
            curstate_ = ClickLbState::Selected;
            // 设置用于样式表的状态属性，应用选中按下的样式
            setProperty("state", selected_press_);
            // 刷新控件样式以应用新的状态样式
            repolish(this);
            // 更新控件显示，确保视觉效果立即生效
            update();

        } else {
            // 从选中状态切换回正常状态
            curstate_ = ClickLbState::Normal;
            // 设置用于样式表的状态属性，应用正常按下的样式
            setProperty("state", normal_press_);
            // 刷新控件样式以应用新的状态样式
            repolish(this);
            // 更新控件显示，确保视觉效果立即生效
            update();
        }
        // 处理完左键点击后直接返回，不再执行基类的默认处理
        return;
    }
    // 对于非左键点击，调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(event);
}

// 处理鼠标释放事件
void ClickedLabel::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		if (curstate_ == ClickLbState::Normal) {
			setProperty("state", normal_hover_);
			repolish(this);
			update();

		} else {
			setProperty("state", selected_hover_);
			repolish(this);
			update();
		}
		emit SigClicked(this->text(), curstate_);
		return;
	}
	// 调用基类的mousePressEvent以保证正常的事件处理
	QLabel::mousePressEvent(event);
}

// 处理鼠标悬停进入事件
void ClickedLabel::enterEvent(QEnterEvent* event) {
	// 在这里处理鼠标悬停进入的逻辑
	if (curstate_ == ClickLbState::Normal) {
		// qDebug()<<"enter , change to normal hover: "<< normal_hover_;
		setProperty("state", normal_hover_);
		repolish(this);
		update();

	} else {
		// qDebug()<<"enter , change to selected hover: "<< selected_hover_;
		setProperty("state", selected_hover_);
		repolish(this);
		update();
	}

	QLabel::enterEvent(static_cast<QEnterEvent*>(event));
}

// 处理鼠标悬停离开事件
void ClickedLabel::leaveEvent(QEvent* event) {
	// 在这里处理鼠标悬停离开的逻辑
	if (curstate_ == ClickLbState::Normal) {
		// qDebug()<<"leave , change to normal : "<< normal_;
		setProperty("state", normal_);
		repolish(this);
		update();

	} else {
		// qDebug()<<"leave , change to normal hover: "<< selected_;
		setProperty("state", selected_);
		repolish(this);
		update();
	}
	QLabel::leaveEvent(event);
}

// 设置图片地址
void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select,
							QString select_hover, QString select_press) {
	normal_ = normal;
	normal_hover_ = hover;
	normal_press_ = press;

	selected_ = select;
	selected_hover_ = select_hover;
	selected_press_ = select_press;

	// 设置默认状态为正常状态
	setProperty("state", normal);
	repolish(this);
	update();
}

ClickLbState ClickedLabel::GetCurState() { return curstate_; }

// 设置当前标签状态
bool ClickedLabel::SetCurState(ClickLbState state) {
	curstate_ = state;
	if (curstate_ == ClickLbState::Normal) {
		setProperty("state", normal_);
		repolish(this);
		update();
	} else if (curstate_ == ClickLbState::Selected) {
		setProperty("state", selected_);
		repolish(this);
		update();
	}

	return true;
}

// 重置为正常状态
void ClickedLabel::ResetNormalState() {
	curstate_ = ClickLbState::Normal;
	setProperty("state", normal_);
	repolish(this);
	update();
}