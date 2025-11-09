#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H
#include <QDebug>
#include <QLineEdit>


class CustomizeEdit : public QLineEdit {
	Q_OBJECT
public:
	CustomizeEdit(QWidget *parent = nullptr);
	void SetMaxLength(int maxLen);

protected:
    // 重写失去焦点事件
	void focusOutEvent(QFocusEvent *event) override {
		// 执行失去焦点时的处理逻辑
		// qDebug() << "CustomizeEdit focusout";
		// 调用基类的focusOutEvent()方法，保证基类的行为得到执行
		QLineEdit::focusOutEvent(event);
		// 发送失去焦点得信号
		emit sig_foucus_out();
	}

private:
    // 限制文本长度
	void LimitTextLength(QString text) {
		if (max_len_ <= 0) {
			return;
		}

		QByteArray byteArray = text.toUtf8();

		if (byteArray.size() > max_len_) {
			byteArray = byteArray.left(max_len_);
			this->setText(QString::fromUtf8(byteArray));
		}
	}
private:
	int max_len_;
signals:
	void sig_foucus_out();
};

#endif	// CUSTOMIZEEDIT_H
