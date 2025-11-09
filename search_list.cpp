#include "search_list.h"
#include <QEvent>
#include <QWheelEvent>
#include <QCoreApplication>
#include <QScrollBar>
#include <QTimer>
#include <iostream>
#include "search_user_item.h"
#include "apply_friend_dialog.h"

SearchList::SearchList(QWidget *parent)
	: QListWidget(parent)
//   _find_dlg(nullptr),
//   _search_edit(nullptr),
/*_send_pending(false)*/ {
    Q_UNUSED(parent);
	// 禁用水平滚动条
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// 禁用垂直滚动条
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// 安装事件过滤器
	this->viewport()->installEventFilter(this);
    // 设置滚动条样式表
    this->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical { background: transparent; width: 6px; margin: 5px 0; }"
        "QScrollBar::handle:vertical { background: rgba(173,170,169,70); min-height: 30px; border-radius: 3px; }"
        "QScrollBar::handle:vertical:hover { background: rgba(173,170,169,150); }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
    );
	// 连接点击的信号和槽
	connect(this, &QListWidget::itemClicked, this, &SearchList::SlotItemClicked);
	// 添加条目
	// addTipItem();
	// 连接搜索条目
	// connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this,
	// &SearchList::slot_user_search);
}

void SearchList::SlotItemClicked(QListWidgetItem* item) {
	// 获取被点击 item 对应的 SearchUserItem 控件
	SearchUserItem* searchUserItem =
		static_cast<SearchUserItem*>(itemWidget(item));
	if (searchUserItem) {
		// 这里可以获取用户信息并进行相应处理
		// 例如显示聊天窗口、切换会话等
		qDebug() << "点击了用户: " << searchUserItem->GetInfo()->name_;
		std::cout << "点击了用户" << std::endl;

		ApplyFriendDialog* apply_friend_dialog = new ApplyFriendDialog(this);
		apply_friend_dialog->SetInfo(searchUserItem->GetInfo());
		apply_friend_dialog->show();

		// 可以根据需要发送信号或执行其他操作
		// emit SigSwitchChatSession(userInfo);
	}
}

bool SearchList::eventFilter(QObject *watched, QEvent *event) {
	// 检查事件是否是鼠标悬浮进入或离开
	if (watched == this->viewport()) {
		if (event->type() == QEvent::Enter) {
			// 鼠标悬浮，显示滚动条
			this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		} else if (event->type() == QEvent::Leave) {
			// 鼠标离开，隐藏滚动条
			this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		}
	}

	// 检查事件是否是鼠标滚轮事件
	if (watched == this->viewport() && event->type() == QEvent::Wheel) {
		// 转换为滚轮事件以获取滚轮角度信息
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
        // 计算滚动度数（通常每格8度）
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        // 计算滚动步数（通常每15度为一步）
        int numSteps = numDegrees / 15;

        // 根据滚动步数调整当前滚动条位置
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() -
                                            numSteps);

        // 检查是否滚动到底部，准备加载更多联系人
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();  // 获取滚动条最大滚动值
        int currentValue = scrollBar->value();       // 获取当前滚动值

        // 当滚动到最底部时
        if (maxScrollValue - currentValue <= 0) {
            // 注释掉的代码段：检查是否已加载完成所有聊天内容
            // auto b_loaded = UserMgr::GetInstance()->IsLoadChatFin();
            // if (b_loaded) {
            //     return true;
            // }

            // 防止重复加载（加锁机制）
            // if (load_pending_) {
            //     return true;  // 如果已有加载任务在进行中，则直接返回
            // }
            
            // 打印调试信息，提示正在加载更多聊天用户
            qDebug() << "load more chat user";
            // 设置加载中标志
            // load_pending_ = true;

            // 使用定时器延迟100毫秒后重置加载标志并退出应用程序
            // 注意：此处退出应用程序的代码可能是调试用的，实际应用中应移除
            // QTimer::singleShot(100, [this]() {
            //     load_pending_ = false;
            //     QCoreApplication::quit();  // 完成后退出应用程序
            // });
            
            // 发送信号通知聊天界面加载更多聊天内容
            emit SigLoadingSearchUser();
        }

        return true;  // 停止事件传递，不执行默认的滚轮行为
	}

	return QListWidget::eventFilter(watched, event);
}