#include "friend_list.h"

#include <QCoreApplication>
#include <QEvent>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <iostream>
#include <memory>

#include "friend_item.h"
#include "group_tip_item.h"
#include "net/tcp_mgr.h"
#include "user_mgr.h"

FriendList::FriendList(QWidget *parent) : QListWidget(parent) {
	Q_UNUSED(parent);
	SetUI();
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

	connect(this, &FriendList::itemClicked, this, &FriendList::SlotItemClicked);
	// 链接自己点击同意认证后界面刷新
	connect(TcpMgr::GetInstance().get(), &TcpMgr::SigAuthRsp, this,
			&FriendList::SlotAuthRsp);
	AddFriendList();
}

void FriendList::AddFriendList() {
	// 添加好友
	auto friend_list = UserMgr::GetInstance()->GetFriendList();
	std::cout << "friend_list size: " << friend_list.size() << std::endl;
	for (auto &friend_info : friend_list) {
		auto *friend_item = new FriendItem();
		friend_item->SetInfo(friend_info->uid_, friend_info->name_, friend_info->icon_);
		QListWidgetItem *item = new QListWidgetItem;
		item->setSizeHint(friend_item->sizeHint());

		// 获取 groupitem 的索引
		int index = this->row(group_item_);
		// 在 groupitem 之后插入新项
		this->insertItem(index + 1, item);

		this->setItemWidget(item, friend_item);
	}
}

void FriendList::SlotItemClicked(QListWidgetItem *item) {
	QWidget *widget = this->itemWidget(item);  // 获取自定义widget对象
	if (!widget) {
		qDebug() << "slot item clicked widget is nullptr";
		return;
	}

	// 对自定义widget进行操作， 将item 转化为基类ListItemBase
	ListItemBase *customItem = qobject_cast<ListItemBase *>(widget);
	if (!customItem) {
		qDebug() << "slot item clicked widget is nullptr";
		return;
	}

	auto itemType = customItem->GetItemType();
	if (itemType == ListItemType::INVALID_ITEM ||
		itemType == ListItemType::GROUP_TIP_ITEM) {
		qDebug() << "slot invalid item clicked ";
		return;
	}

	if (itemType == ListItemType::APPLY_FRIEND_ITEM) {
		// 创建对话框，提示用户
		qDebug() << "apply friend item clicked ";
		std::cout << "apply friend item clicked " << std::endl;
		// 跳转到好友申请界面
		emit SigSwitchApplyFriendWidget();
		return;
	}

	if (itemType == ListItemType::CONTACT_USER_ITEM) {
		// 创建对话框，提示用户
		qDebug() << "contact user item clicked ";
		std::cout << "contact user item clicked " << std::endl;

		auto friend_item = qobject_cast<FriendItem *>(customItem);
		auto user_info = friend_item->GetUserInfo();
		qDebug() << "friend list info: " << user_info->uid_ << ' ' << user_info->name_;
		// 将该项添加进聊天列表中
		// TODO: 目前进行实现点击好友项后，跳转到聊天界面
		emit SigSwitchChat(user_info);
		// 跳转到好友申请界面
		//    emit sig_switch_friend_info_page(user_info);
		return;
	}
}

void FriendList::SlotAuthRsp(std::shared_ptr<AuthRsp> auth_rsp) {
	qDebug() << "slot auth rsp called";
	bool isFriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->uid_);
	if (isFriend) {
		return;
	}

	auto user_info = std::make_shared<UserInfo>(auth_rsp->uid_, auth_rsp->name_, "", auth_rsp->icon_, auth_rsp->gender_);

	UserMgr::GetInstance()->AddFriend(user_info);

	// 好友项
	auto *con_user_wid = new FriendItem();
	con_user_wid->SetInfo(auth_rsp->uid_, auth_rsp->name_, auth_rsp->icon_);
	QListWidgetItem *item = new QListWidgetItem;
	item->setSizeHint(con_user_wid->sizeHint());

	// 获取 groupitem 的索引
	int index = this->row(group_item_);
	// 在 groupitem 之后插入新项
	this->insertItem(index + 1, item);

	this->setItemWidget(item, con_user_wid);
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
bool FriendList::eventFilter(QObject *watched, QEvent *event) {
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
			//     return true;  // 如果已有加载任务在进行中，则直接返回
			// }

			// 设置加载中标志
			// load_pending_ = true;

			// 使用定时器延迟100毫秒后重置加载标志并退出应用程序
			// 注意：此处退出应用程序的代码可能是调试用的，实际应用中应移除
			// QTimer::singleShot(100, [this]() {
			//     load_pending_ = false;
			//     QCoreApplication::quit();  // 完成后退出应用程序
			// });

			// 发送信号通知好友界面加载更多好友
			emit SigLoadingFriend();
		}

		return true;  // 停止事件传递，不执行默认的滚轮行为
	}

	// 对于其他未处理的事件，调用父类的事件过滤器处理
	return QListWidget::eventFilter(watched, event);
}

void FriendList::SetUI() {
	auto *groupTip = new GroupTipItem();
	groupTip->SetGroupTip("新的朋友");

	QListWidgetItem *item = new QListWidgetItem;
	item->setSizeHint(groupTip->sizeHint());
	this->addItem(item);
	this->setItemWidget(item, groupTip);
	item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

	// 申请好友项
	apply_item_ = new FriendItem();
	apply_item_->setObjectName("new_friend_item");
	apply_item_->SetInfo("", "新的朋友", "../res/add_friend.png");
	apply_item_->SetItemType(ListItemType::APPLY_FRIEND_ITEM);

	QListWidgetItem *add_item = new QListWidgetItem;
	add_item->setSizeHint(apply_item_->sizeHint());
	this->addItem(add_item);
	this->setItemWidget(add_item, apply_item_);
	// 默认设置新的朋友申请条目被选中
	//  this->setCurrentItem(add_item);

	auto *groupCon = new GroupTipItem();
	groupCon->SetGroupTip(tr("联系人"));
	group_item_ = new QListWidgetItem;
	group_item_->setSizeHint(groupCon->sizeHint());
	this->addItem(group_item_);
	this->setItemWidget(group_item_, groupCon);
	group_item_->setFlags(group_item_->flags() & ~Qt::ItemIsSelectable);
}
