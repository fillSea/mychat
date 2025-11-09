#ifndef STATE_WIDGET_H
#define STATE_WIDGET_H
#include <QLabel>
#include <QWidget>
#include "../const.h"

// 状态按钮
class StateWidget : public QWidget {
	Q_OBJECT
public:
	explicit StateWidget(QWidget* parent = nullptr);

	void SetState(QString normal = "", QString hover = "", QString press = "",
				  QString select = "", QString select_hover = "",
				  QString select_press = "");

	ClickLbState GetCurState();
	void ClearState();

	void SetSelected(bool bselected);
	void AddRedPoint();
	void ShowRedPoint(bool show = true);

protected:
	void paintEvent(QPaintEvent* event) override;

	virtual void mousePressEvent(QMouseEvent* ev) override;
	virtual void mouseReleaseEvent(QMouseEvent* ev) override;
	virtual void enterEvent(QEnterEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;

private:
	QString normal_;
	QString hover_;
	QString press_;

	QString select_;
	QString select_hover_;
	QString select_press_;

	ClickLbState cur_state_;
	QLabel* red_point_;

signals:
	void clicked(void);

signals:

public slots:
};

#endif	// STATE_WIDGET_H