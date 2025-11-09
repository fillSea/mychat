#include "customize_edit.h"

CustomizeEdit::CustomizeEdit(QWidget *parent) : QLineEdit(parent), max_len_(0) {
	connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::LimitTextLength);
}

void CustomizeEdit::SetMaxLength(int maxLen) { max_len_ = maxLen; }