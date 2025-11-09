#include "chat_message_show_widget.h"

ChatMessageShowWidget::ChatMessageShowWidget(QWidget* parent) : QWidget(parent) {
	SetUI();
}

ChatMessageShowWidget::~ChatMessageShowWidget() {}

void ChatMessageShowWidget::SetUI() {
	QVBoxLayout *main_layout = new QVBoxLayout();
    this->setLayout(main_layout);
    main_layout->setContentsMargins(0, 0, 0, 0);

    scroll_area_ = new QScrollArea();
    scroll_area_->setObjectName("chat_area");
    main_layout->addWidget(scroll_area_);

    QWidget *w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);
    QVBoxLayout *pHLayout_1 = new QVBoxLayout();
    pHLayout_1->addWidget(new QWidget(), 100000);
    w->setLayout(pHLayout_1);
    scroll_area_->setWidget(w);    //应该时在QSCrollArea构造后执行 才对

    scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = scroll_area_->verticalScrollBar();
    // connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatMessageShowWidget::onVScrollBarMoved);
    //把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setContentsMargins(0, 0, 0, 0);
    scroll_area_->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    scroll_area_->setWidgetResizable(true);
    scroll_area_->installEventFilter(this);
}