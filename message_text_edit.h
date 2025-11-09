#ifndef MESSAGE_TEXT_EDIT_H
#define MESSAGE_TEXT_EDIT_H
#include <QTextEdit>

#include "const.h"


class MessageTextEdit : public QTextEdit {
	Q_OBJECT
public:
	explicit MessageTextEdit(QWidget *parent = nullptr);

	~MessageTextEdit();

	QVector<MsgInfo> GetMsgList();

	// void insertFileFromUrl(const QStringList &urls);
signals:
	// void send();

protected:
	// void dragEnterEvent(QDragEnterEvent *event);
	// void dropEvent(QDropEvent *event);
	// void keyPressEvent(QKeyEvent *e);

private:
	// void insertImages(const QString &url);
	// void insertTextFile(const QString &url);
	// bool canInsertFromMimeData(const QMimeData *source) const;
	// void insertFromMimeData(const QMimeData *source);
	// bool isImage(QString url);//判断文件是否为图片
	void InsertMsgList(QVector<MsgInfo> &list,QString flag, QString text, QPixmap pix);

	// QStringList getUrl(QString text);
	// QPixmap getFileIconPixmap(const QString
	// &url);//获取文件图标及大小信息，并转化成图片 QString getFileSize(qint64
	// size);//获取文件大小

private slots:
	// void textEditChanged();

private:
	QVector<MsgInfo> msg_list_;
	QVector<MsgInfo> get_msg_list_;
};

#endif	// MESSAGE_TEXT_EDIT_H
