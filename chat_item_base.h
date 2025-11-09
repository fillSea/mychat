#ifndef CHAT_ITEM_H
#define CHAT_ITEM_H
#include <QLabel>
#include <QWidget>

#include "const.h"


class ChatItemBase : public QWidget {
	Q_OBJECT
public:
	explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);
	void setUserName(const QString &name);
	void setUserIcon(const QPixmap &icon);
	void setWidget(QWidget *w);
private:
	void SetUI();
private:
	ChatRole role_;
	QLabel *name_label_;
	QLabel *icon_label_;
	QWidget *bubble_;
};

#endif	// CHAT_ITEM_H