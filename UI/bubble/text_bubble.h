#ifndef TEXT_BUBBLE_H
#define TEXT_BUBBLE_H
#include <QTextEdit>

#include "bubble_frame.h"


// 文本气泡框
class TextBubble : public BubbleFrame {
	Q_OBJECT
public:
	TextBubble(ChatRole role, const QString &text, QWidget *parent = nullptr);

protected:
	bool eventFilter(QObject *o, QEvent *e);

private:
	void AdjustTextHeight();
	void SetPlainText(const QString &text);
	void InitStyleSheet();
    void SetUI(const QString& text);

private:
	QTextEdit *text_edit_;//文本编辑框
};

#endif	// TEXT_BUBBLE_H