#include "text_bubble.h"

#include <QFontMetricsF>
#include <QTextBlock>
#include <QTextDocument>
#include <algorithm>

TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent)
	: BubbleFrame(role, parent) {
	SetUI(text);
}

void TextBubble::SetPlainText(const QString &text) {
	text_edit_->setPlainText(text);

	// 设置一行显示的文本宽度
	// 找到段落中最大宽度
	qreal doc_margin = text_edit_->document()->documentMargin();
	// 内容区域的左右内边距
	int margin_left = this->layout()->contentsMargins().left();
	int margin_right = this->layout()->contentsMargins().right();
	// 创建一个字体度量对象，用于计算文本的宽度
	QFontMetricsF fm(text_edit_->font());
	// 获取文本编辑器的文档对象，用于后续遍历文本块
	QTextDocument *doc = text_edit_->document();
	int max_width = 0;
	// 遍历文档中的每一个文本块，找到最宽的那一段
	for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {	// 字体总长
		// 计算每个文本块的水平宽度
		int txtW = int(fm.horizontalAdvance(it.text()));
		max_width = std::max(max_width, txtW);	// 找到最长的那段
	}

	// 添加一个合理的最大宽度限制，例如设置为250像素
    const int REASONABLE_MAX_WIDTH = 250;
    max_width = std::min(std::max(max_width, REASONABLE_MAX_WIDTH), REASONABLE_MAX_WIDTH);

	// 设置这个气泡的最大宽度 只需要设置一次
	setMaximumWidth(max_width + doc_margin * 2 +
					(margin_left + margin_right));	// 设置最大宽度
}

bool TextBubble::eventFilter(QObject *o, QEvent *e) {
	if (text_edit_ == o && e->type() == QEvent::Paint) {
		AdjustTextHeight();	 // 调整文本高度
	}
	return BubbleFrame::eventFilter(o, e);
}

// 调整文本高度
void TextBubble::AdjustTextHeight() {
	// 字体到边框的距离默认为4
    //本内容到文档边界的距离
	qreal doc_margin = text_edit_->document()->documentMargin();
	QTextDocument *doc = text_edit_->document();
	qreal text_height = 0;// 总的文本高度
	// 把每一段的高度相加=文本高
	for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
		QTextLayout *pLayout = it.layout();
		QRectF text_rect = pLayout->boundingRect();	 // 这段的rect
		text_height += text_rect.height();
	}
	int vMargin = this->layout()->contentsMargins().top();
	// 设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
	setFixedHeight(text_height + doc_margin * 2 + vMargin * 2);
}

void TextBubble::SetUI(const QString &text) {
	text_edit_ = new QTextEdit();
	text_edit_->setReadOnly(true);
	// 禁用滚动条
	text_edit_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	text_edit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	text_edit_->installEventFilter(this);

	QFont font("Microsoft YaHei");
	font.setPointSize(12);

	text_edit_->setFont(font);

	SetPlainText(text);
	// 设置气泡的内容
	SetWidget(text_edit_);
	InitStyleSheet();
}

void TextBubble::InitStyleSheet() {
	text_edit_->setStyleSheet("QTextEdit{background:transparent;border:none}");
}