#include "apply_friend_list.h"

#include <QCoreApplication>
#include <QEvent>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>


ApplyFriendList::ApplyFriendList(QWidget *parent) : QListWidget(parent) {
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
		"QScrollBar::handle:vertical { background: rgba(173,170,169,70); min-height: "
		"30px; border-radius: 3px; }"
		"QScrollBar::handle:vertical:hover { background: rgba(173,170,169,150); }"
		"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: "
		"none; }");
}

/**
 * @brief ChatUserList::eventFilter
 * @param watched 被监视的对象
 * @param event 发生的事件
 * @return 事件是否被处理（true表示事件已处理，不再传递；false表示继续传递事件）
 *
 * 自定义事件过滤器，用于处理用户列表控件的特殊事件行为：
 * 1. 鼠标悬浮显示/隐藏滚动条
 * 2. 自定义鼠标滚轮滚动行为
 * 3. 检测滚动到底部并触发加载更多联系人的操作
 */
bool ApplyFriendList::eventFilter(QObject *watched, QEvent *event) {
	// 检查事件是否是鼠标悬浮进入或离开视口区域
	if (watched == this->viewport()) {
		if (event->type() == QEvent::Enter) {
			// 鼠标悬浮进入视口，显示垂直滚动条
			this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		} else if (event->type() == QEvent::Leave) {
			// 鼠标离开视口，隐藏垂直滚动条
			this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		}
	}

	if (watched == this->viewport()) {
		if (event->type() == QEvent::MouseButtonPress) {
			// emit sig_show_search(false);
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
		int maxScrollValue = scrollBar->maximum();	// 获取滚动条最大滚动值
		int currentValue = scrollBar->value();		// 获取当前滚动值

		// 当滚动到最底部时
		if (maxScrollValue - currentValue <= 0) {
			// 注释掉的代码段：检查是否已加载完成所有聊天内容
			// auto b_loaded = UserMgr::GetInstance()->IsLoadChatFin();
			// if (b_loaded) {
			//     return true;
			// }

			// 防止重复加载（加锁机制）
			// if (load_pending_) {
			// return true;  // 如果已有加载任务在进行中，则直接返回
			// }

			// 设置加载中标志
			// load_pending_ = true;

			// 使用定时器延迟100毫秒后重置加载标志并退出应用程序
			// 注意：此处退出应用程序的代码可能是调试用的，实际应用中应移除
			// QTimer::singleShot(100, [this]() {
			//     load_pending_ = false;
			//     QCoreApplication::quit();  // 完成后退出应用程序
			// });

			// 发送信号通知聊天界面加载更多聊天内容
			emit SigLoadingApplyUser();
		}

		return true;  // 停止事件传递，不执行默认的滚轮行为
	}

	// 对于其他未处理的事件，调用父类的事件过滤器处理
	return QListWidget::eventFilter(watched, event);
}