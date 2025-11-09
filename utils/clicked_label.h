#ifndef CLICKED_LABEL_H
#define CLICKED_LABEL_H
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>

#include "../const.h"


class ClickedLabel : public QLabel {
	Q_OBJECT
public:
	ClickedLabel(QWidget* parent);
	virtual void mousePressEvent(QMouseEvent* ev) override;
	virtual void mouseReleaseEvent(QMouseEvent* ev) override;
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	// 设置标签的状态图片地址
	void SetState(QString normal = "", QString hover = "", QString press = "",
				  QString select = "", QString select_hover = "",
				  QString select_press = "");

	// 获取当前标签状态
	ClickLbState GetCurState();
	// 设置当前标签状态
	bool SetCurState(ClickLbState state);
	// 重置为正常状态
	void ResetNormalState();

signals:
    // 标签点击信号
    void SigClicked(QString, ClickLbState);
private:
	// 各状态的图片地址
	QString normal_;
	QString normal_hover_;
	QString normal_press_;

	QString selected_;
	QString selected_hover_;
	QString selected_press_;

	ClickLbState curstate_;
};

#endif	// CLICKED_LABEL_H