#include "chat_message_edit_widget.h"

#include <QVBoxLayout>
#include <QWidget>



ChatMessageEditWidget::ChatMessageEditWidget(QWidget* parent) : QWidget(parent) {
	SetUI();

	connect(send_btn_, &QPushButton::clicked, this, &ChatMessageEditWidget::SigSendMsg);
}


void ChatMessageEditWidget::SetUI() {
	setMinimumHeight(50);	// 保证最小显示空间
	setMaximumHeight(200);	// 限制最大高度，避免过高
							// setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setSizePolicy(QSizePolicy::Expanding,
				  QSizePolicy::Preferred);	// 垂直方向优先适应内容

	// 垂直方向布局管理器
	QVBoxLayout* vlayout = new QVBoxLayout(this);
	vlayout->setSpacing(5);
	// vlayout->setContentsMargins(15, 5, 15, 10);
	vlayout->setContentsMargins(10, 10, 10, 10);  // 减小外边框，更紧凑
	setLayout(vlayout);

	// 水平方向布局管理器
	QHBoxLayout* hlayout = new QHBoxLayout(this);
	hlayout->setSpacing(8);
	hlayout->setContentsMargins(0, 0, 0, 0);
	hlayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	vlayout->addLayout(hlayout);

	// 把上方的四个按钮, 创建好并添加到水平布局中
	static const QString btnStyle =
		"QPushButton {"
		"    background-color: rgba(240, 243, 250, 50);"
		"    border: none;"
		"    border-radius: 4px;"
		"}"
		"QPushButton:hover {"
		"    background-color: rgba(200, 210, 230, 80);"
		"}"
		"QPushButton:pressed {"
		"    background-color: rgba(180, 190, 210, 100);"
		"}";
	// static const QSize btnSize(36, 36);
	// static const QSize iconSize(22, 22);
	static const QSize btnSize(34, 34);	 // 略小按钮，更精致
	static const QSize iconSize(20, 20);

	// 发送图片按钮
	send_image_btn_ = new QPushButton();
	send_image_btn_->setFixedSize(btnSize);
	send_image_btn_->setIconSize(iconSize);
	send_image_btn_->setIcon(QIcon("../res/image.png"));
	send_image_btn_->setStyleSheet(btnStyle);
	send_image_btn_->setToolTip("发送图片");  // 添加提示
	hlayout->addWidget(send_image_btn_);

	// 发送文件按钮
	send_file_btn_ = new QPushButton();
	send_file_btn_->setFixedSize(btnSize);
	send_file_btn_->setIconSize(iconSize);
	send_file_btn_->setIcon(QIcon("../res/file.png"));
	send_file_btn_->setStyleSheet(btnStyle);
	send_file_btn_->setToolTip("发送文件");
	hlayout->addWidget(send_file_btn_);

	// 发送语音按钮
	send_speech_btn_ = new QPushButton();
	send_speech_btn_->setFixedSize(btnSize);
	send_speech_btn_->setIconSize(iconSize);
	send_speech_btn_->setIcon(QIcon("../res/sound.png"));
	send_speech_btn_->setStyleSheet(btnStyle);
	send_speech_btn_->setToolTip("发送语音");
	hlayout->addWidget(send_speech_btn_);

	// 显示历史消息按钮
	// showHistoryBtn = new QPushButton();
	// showHistoryBtn->setFixedSize(btnSize);
	// showHistoryBtn->setIconSize(iconSize);
	// showHistoryBtn->setIcon(QIcon(":/resource/image/history.png"));
	// showHistoryBtn->setStyleSheet(btnStyle);
	// hlayout->addWidget(showHistoryBtn);

	// 多行编辑框
	text_edit_ = new MessageTextEdit();
	text_edit_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	text_edit_->setStyleSheet(
		"QTextEdit {"
		"    border: none;"
		"    font-size: 14px;"
		"}"
		"QScrollBar:vertical {"
		"    width: 6px;"
		"    background: transparent;"
		"    margin: 2px 0;"
		"}"
		"QScrollBar::handle:vertical {"
		"    background: rgba(180, 190, 210, 150);"
		"    min-height: 30px;"
		"    border-radius: 3px;"
		"}"
		"QScrollBar::handle:vertical:hover {"
		"    background: rgba(160, 170, 190, 180);"
		"}"
		"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
		"    height: 0px;"
		"    background: none;"
		"}");
	vlayout->addWidget(text_edit_);

	// 提示 "录制中" QLabel
	// tipLabel = new QLabel();
	// tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	// tipLabel->setText("录音中...");
	// tipLabel->setAlignment(Qt::AlignCenter);
	// tipLabel->setFont(QFont("微软雅黑", 18, 500));
	// tipLabel->setStyleSheet("color: rgb(100, 100, 100);");
	// vlayout->addWidget(tipLabel);
	// tipLabel->hide();

	// 添加发送文本消息按钮
	send_btn_ = new QPushButton();
	send_btn_->setText("发送");
	send_btn_->setFixedSize(100, 36);
	static const QString style =
		"QPushButton {"
		"    font-size: 14px;"
		"    color: white;"
		"    border: none;"
		"    background-color: rgb(100, 160, 220);"
		"    border-radius: 6px;"
		"}"
		"QPushButton:hover {"
		"    background-color: rgb(90, 150, 210);"
		"}"
		"QPushButton:pressed {"
		"    background-color: rgb(80, 140, 200);"
		"}";
	send_btn_->setStyleSheet(style);
	vlayout->addWidget(send_btn_, 0, Qt::AlignRight | Qt::AlignVCenter);
}
