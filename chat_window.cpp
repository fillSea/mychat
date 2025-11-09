#include "chat_window.h"

#include <qlistwidget.h>
#include <qpushbutton.h>

#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

#include "chat_user_item.h"
#include "chat_user_list.h"
#include "friend_item.h"
#include "net/tcp_mgr.h"
#include "search_user_item.h"
#include "user_mgr.h"
#include "utils/clicked_btn.h"
#include "utils/util.h"

// TODO: 聊天消息发送
ChatWindow::ChatWindow(QWidget* parent)
	: QWidget(parent),
	  b_loading_(false),
	  mode_(ChatUIMode::ChatMode),
	  state_(ChatUIMode::ChatMode) {
	SetUI();
	// add_friend_btn_->SetState("normal", "hover", "press");
	// add_friend_btn_->setProperty("state", "normal");
	session_tab_label_->SetState("normal", "hover", "pressed", "selected_normal",
								 "selected_hover", "selected_pressed");

	friend_tab_label_->SetState("normal", "hover", "pressed", "selected_normal",
								"selected_hover", "selected_pressed");

	// 添加标签组
	AddLBGroup(session_tab_label_);
	// AddLBGroup(apply_tab_label_);
	AddLBGroup(friend_tab_label_);

	ShowSearch(false);
	// addChatUserList();
	// addSearchUserList();
	// ShowSearch(true);
	// addFriendList();

	// 连接加载信号和槽
	connect(chat_user_list_, &ChatUserList::SigLoadingChatUser, this,
			&ChatWindow::SlotLoadingChatUser);
	connect(search_list_, &SearchList::SigLoadingSearchUser, this,
			&ChatWindow::SlotLoadingSearchUser);
	connect(friend_list_, &FriendList::SigLoadingFriend, this,
			&ChatWindow::SlotLoadingFriend);
	connect(session_tab_label_, &StateWidget::clicked, this,
			&ChatWindow::SlotSwitchTabChat);
	connect(friend_tab_label_, &StateWidget::clicked, this,
			&ChatWindow::SlotSwitchTabFriend);
	// 连接搜索按钮点击事件
	connect(search_btn_, &ClickedBtn::clicked, this, &ChatWindow::SlotSearch);
	// 连接搜索完成信号
	connect(TcpMgr::GetInstance().get(), &TcpMgr::SigGetSearchUsers, this,
			&ChatWindow::SlotGetSearchUsers);
	connect(TcpMgr::GetInstance().get(), &TcpMgr::SigGetSearchUser, this,
			&ChatWindow::SlotGetSearchUser);
	// 连接搜索框输入变化(这里改成点击搜索按钮后，才触发搜索, 切换成 search_list)
	// connect(search_edit_, &QLineEdit::textChanged, this, &ChatWindow::SlotTextChanged);
	// 连接联系人页面点击好友申请条目的信号
	connect(friend_list_, &FriendList::SigSwitchApplyFriendWidget, this,
			&ChatWindow::SlotSwitchApplyFriendWidget);
	// 连接好友列表点击好友项的信号
	connect(friend_list_, &FriendList::SigSwitchChat, this, &ChatWindow::SlotSwitchChat);
	// 连接对端消息通知
	connect(TcpMgr::GetInstance().get(), &TcpMgr::SigTextChatMsg, this,
			&ChatWindow::SlotTextChatMsg);
}

ChatWindow::~ChatWindow() {}

// 测试数据, 模拟加载聊天记录
void ChatWindow::addChatUserList() {
	// 先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
	// auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
	// if (friend_list.empty() == false) {
	// 	for (auto& friend_ele : friend_list) {
	// 		auto find_iter = _chat_items_added.find(friend_ele->_uid);
	// 		if (find_iter != _chat_items_added.end()) {
	// 			continue;
	// 		}
	// 		auto* chat_user_wid = new ChatUserWid();
	// 		auto user_info = std::make_shared<UserInfo>(friend_ele);
	// 		chat_user_wid->SetInfo(user_info);
	// 		QListWidgetItem* item = new QListWidgetItem;
	// 		// qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
	// 		item->setSizeHint(chat_user_wid->sizeHint());
	// 		ui->chat_user_list->addItem(item);
	// 		ui->chat_user_list->setItemWidget(item, chat_user_wid);
	// 		_chat_items_added.insert(friend_ele->_uid, item);
	// 	}

	// 	// 更新已加载条目
	// 	UserMgr::GetInstance()->UpdateChatLoadedCount();
	// }

	// 模拟测试条目
	//  创建QListWidgetItem，并设置自定义的widget
	for (int i = 0; i < 13; i++) {
		int randomValue =
			QRandomGenerator::global()->bounded(100);  // 生成0到99之间的随机整数
		int str_i = randomValue % strs.size();
		int head_i = randomValue % heads.size();
		int name_i = randomValue % names.size();

		auto* chat_user_item = new ChatUserItem();
		auto user_info = std::make_shared<UserInfo>("", names[name_i], names[name_i],
													heads[head_i], 0, strs[str_i]);
		chat_user_item->SetInfo(user_info);
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(chat_user_item->sizeHint());
		chat_user_list_->addItem(item);
		chat_user_list_->setItemWidget(item, chat_user_item);
	}
}

// 测试数据, 模拟加载搜索用户列表
void ChatWindow::addSearchUserList() {
	// 模拟测试条目
	//  创建QListWidgetItem，并设置自定义的widget
	for (int i = 0; i < 13; i++) {
		int randomValue =
			QRandomGenerator::global()->bounded(100);  // 生成0到99之间的随机整数
		int str_i = randomValue % strs.size();
		int head_i = randomValue % heads.size();
		int name_i = randomValue % names.size();

		auto* chat_user_item = new SearchUserItem();
		auto search_info = std::make_shared<SearchInfo>(
			"123456", names[name_i], "123456@163.com", 0, heads[head_i], "", "");
		chat_user_item->SetInfo(search_info);
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(chat_user_item->sizeHint());
		search_list_->addItem(item);
		search_list_->setItemWidget(item, chat_user_item);
	}
}

// 切换到聊天窗口
void ChatWindow::SlotSwitchChat(std::shared_ptr<UserInfo> user_info) {
	// 判断如果已经添加过则跳过
	auto is_added = chat_items_added_.find(user_info->uid_);
	if (is_added != chat_items_added_.end()) {
		return;
	}

	// 添加好友
	UserMgr::GetInstance()->AddFriend(user_info);

	auto* chat_user_wid = new ChatUserItem();
	chat_user_wid->SetInfo(user_info);
	qDebug() << "ChatWindow::SlotSwitchChat: " << user_info->uid_;
	QListWidgetItem* item = new QListWidgetItem;
	item->setSizeHint(chat_user_wid->sizeHint());
	chat_user_list_->insertItem(0, item);
	chat_user_list_->setItemWidget(item, chat_user_wid);
	chat_items_added_.insert(user_info->uid_, item);

	ClearLabelState(session_tab_label_);
	chat_widget_->SetUserInfo(user_info);
	stacked_widget_->setCurrentWidget(chat_widget_);
	state_ = ChatUIMode::ChatMode;
	ShowSearch(false);
}

void ChatWindow::SlotTextChatMsg(std::shared_ptr<TextChatMsg> msg) {
	// 查找是否添加了该好友的聊天项
	auto find_iter = chat_items_added_.find(msg->from_uid_);
	qDebug() << "ChatWindow::SlotTextChatMsg: " << msg->from_uid_;
	if (find_iter != chat_items_added_.end()) {
		qDebug() << "set chat item msg, uid is " << msg->from_uid_;
		QWidget* widget = chat_user_list_->itemWidget(find_iter.value());
		auto chat_wid = qobject_cast<ChatUserItem*>(widget);
		if (!chat_wid) {
			return;
		}
		// 设置聊天item
		chat_wid->UpdateLastMsg(msg->chat_msgs_);
		// 更新当前聊天页面记录
		UpdateChatMsg(msg->chat_msgs_);
		UserMgr::GetInstance()->AppendFriendChatMsg(msg->from_uid_, msg->chat_msgs_);
		return;
	}

	/*
	// 如果没找到，则创建新的插入listwidget
	auto* chat_user_wid = new ChatUserItem();
	// 查询好友信息
	auto fi_ptr = UserMgr::GetInstance()->GetFriendById(msg->from_uid_);
	chat_user_wid->SetInfo(fi_ptr);
	QListWidgetItem* item = new QListWidgetItem;
	// qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
	item->setSizeHint(chat_user_wid->sizeHint());
	chat_user_wid->UpdateLastMsg(msg->chat_msgs_);
	UserMgr::GetInstance()->AppendFriendChatMsg(msg->from_uid_, msg->chat_msgs_);
	chat_user_list_->insertItem(0, item);
	chat_user_list_->setItemWidget(item, chat_user_wid);
	chat_items_added_.insert(msg->from_uid_, item);
	*/
}

// 更新聊天消息
void ChatWindow::UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>> msgdata) {
	for (auto& msg : msgdata) {
		// if (msg->from_uid_ != cur_chat_uid_) {
		// 	break;
		// }

		chat_widget_->AppendChatMsg(msg);
	}
}

// 显示界面
void ChatWindow::ShowSearch(bool bsearch) {
	if (bsearch) {
		chat_user_list_->hide();
		friend_list_->hide();
		search_list_->show();
		mode_ = ChatUIMode::SearchMode;
	} else if (state_ == ChatUIMode::ChatMode) {
		chat_user_list_->show();
		friend_list_->hide();
		search_list_->hide();
		mode_ = ChatUIMode::ChatMode;
		// ui->search_list->CloseFindDlg();
		// ui->search_edit->clear();
		// ui->search_edit->clearFocus();
	} else if (state_ == ChatUIMode::ContactMode) {
		chat_user_list_->hide();
		search_list_->hide();
		friend_list_->show();
		mode_ = ChatUIMode::ContactMode;
		// ui->search_list->CloseFindDlg();
		// ui->search_edit->clear();
		// ui->search_edit->clearFocus();
	} else if (state_ == ChatUIMode::SettingsMode) {
		chat_user_list_->hide();
		search_list_->hide();
		friend_list_->show();
		mode_ = ChatUIMode::SettingsMode;
		// ui->search_list->CloseFindDlg();
		// ui->search_edit->clear();
		// ui->search_edit->clearFocus();
	}
}

// 搜索框输入变化
void ChatWindow::SlotTextChanged(const QString& text) {
	if (!text.isEmpty()) {
		ShowSearch(true);
	}
}

// 动态加载聊天列表
void ChatWindow::SlotLoadingChatUser() {
	if (b_loading_) {
		return;
	}

	b_loading_ = true;
	// LoadingDlg *loadingDialog = new LoadingDlg(this);
	// loadingDialog->setModal(true);
	// loadingDialog->show();
	qDebug() << "add new data to list.....";
	// LoadMoreChatUser();
	// 加载完成后关闭对话框
	// loadingDialog->deleteLater();
	addChatUserList();

	b_loading_ = false;
}

// 加载更多聊天用户
void ChatWindow::LoadMoreChatUser() {
	// auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
	// if (friend_list.empty() == false) {
	//     for(auto & friend_ele : friend_list){
	//         auto find_iter = _chat_items_added.find(friend_ele->_uid);
	//         if(find_iter != _chat_items_added.end()){
	//             continue;
	//         }
	//         auto *chat_user_wid = new ChatUserWid();
	//         auto user_info = std::make_shared<UserInfo>(friend_ele);
	//         chat_user_wid->SetInfo(user_info);
	//         QListWidgetItem *item = new QListWidgetItem;
	//         //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
	//         item->setSizeHint(chat_user_wid->sizeHint());
	//         ui->chat_user_list->addItem(item);
	//         ui->chat_user_list->setItemWidget(item, chat_user_wid);
	//         _chat_items_added.insert(friend_ele->_uid, item);
	//     }

	//     //更新已加载条目
	//     UserMgr::GetInstance()->UpdateChatLoadedCount();
	// }
}

void ChatWindow::addFriendList() {
	// 模拟测试条目
	//  创建QListWidgetItem，并设置自定义的widget
	for (int i = 0; i < 13; i++) {
		int randomValue =
			QRandomGenerator::global()->bounded(100);  // 生成0到99之间的随机整数
		int str_i = randomValue % strs.size();
		int head_i = randomValue % heads.size();
		int name_i = randomValue % names.size();

		auto* friend_item = new FriendItem();
		auto user_info = std::make_shared<UserInfo>("", names[name_i], names[name_i],
													heads[head_i], 0, strs[str_i]);
		friend_item->SetUserInfo(user_info);
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(friend_item->sizeHint());
		friend_list_->addItem(item);
		friend_list_->setItemWidget(item, friend_item);
	}
}

void ChatWindow::SlotLoadingFriend() { /*addFriendList();*/ }

// 动态加载搜索用户列表
void ChatWindow::SlotLoadingSearchUser() { addSearchUserList(); }

// 搜索按钮点击事件
void ChatWindow::SlotSearch() {
	QString text = search_edit_->text();
	if (text.isEmpty()) {
		return;
	}
	// 发送查找用户请求
	QJsonObject json_obj;
	// 判断通过user_id 还是 username
	bool is_id = text.toLongLong() != 0;
	if (is_id) {
		json_obj["user_id"] = text.toLongLong();
	} else {
		json_obj["username"] = text;
	}
	std::cout << "search_btn clicked" << std::endl;
	QJsonDocument doc(json_obj);
	QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
	// 发送查询请求
	emit TcpMgr::GetInstance() -> SigSendData(ReqId::SEARCH_USER_REQ, jsonData);
}

// 搜索完成信号槽
void ChatWindow::SlotGetSearchUsers(QVector<std::shared_ptr<SearchInfo>>& users) {}

// 搜索完成信号槽, 通过 user_id 搜索
void ChatWindow::SlotGetSearchUser(std::shared_ptr<SearchInfo> user) {
	// 将搜索结果添加到search_list_中

	auto* chat_user_item = new SearchUserItem();
	auto search_info = std::make_shared<SearchInfo>(user->uid_, user->name_, user->email_,
													0, user->icon_, "", "");
	chat_user_item->SetInfo(search_info);
	QListWidgetItem* item = new QListWidgetItem;
	item->setSizeHint(chat_user_item->sizeHint());
	search_list_->addItem(item);
	search_list_->setItemWidget(item, chat_user_item);
	ShowSearch(true);
}

// 添加标签组
void ChatWindow::AddLBGroup(StateWidget* lb) { label_list_.push_back(lb); }

// 清除所有标签状态
void ChatWindow::ClearLabelState(StateWidget* lb) {
	for (auto& ele : label_list_) {
		if (ele == lb) {
			continue;
		}

		ele->ClearState();
	}
}

void ChatWindow::SlotSwitchTabChat() {
	ClearLabelState(session_tab_label_);
	stacked_widget_->setCurrentWidget(chat_widget_);
	state_ = ChatUIMode::ChatMode;
	ShowSearch(false);
}

// 切换到好友界面
void ChatWindow::SlotSwitchTabFriend() {
	ClearLabelState(friend_tab_label_);
	// 设置
	//  if(_last_widget == nullptr){
	//      ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
	//      _last_widget = ui->friend_apply_page;
	//  }else{
	//      ui->stackedWidget->setCurrentWidget(_last_widget);
	//  }
	stacked_widget_->setCurrentWidget(apply_friend_widget_);
	state_ = ChatUIMode::ContactMode;
	ShowSearch(false);
}

// 切换到好友申请界面
void ChatWindow::SlotSwitchApplyFriendWidget() {
	// 切换到好友申请界面
	stacked_widget_->setCurrentWidget(apply_friend_widget_);
	state_ = ChatUIMode::ContactMode;
	ShowSearch(false);
}

void ChatWindow::SetUI() {
	resize(1000, 700);	// 窗口初始大小
	InitMainWindow();
	InitLeftWindow();
	InitMidWindow();
	InitRightWindow();

	QAction* search_action = new QAction(search_edit_);

	search_action->setIcon(QIcon("../res/search.png"));
	search_edit_->addAction(search_action, QLineEdit::LeadingPosition);
	search_edit_->setPlaceholderText(QStringLiteral("搜索"));

	// 创建一个清除动作并设置图标
	QAction* clear_action = new QAction(search_edit_);
	clear_action->setIcon(QIcon("../res/close_transparent.png"));
	// 初始时不显示清除图标
	// 将清除动作添加到LineEdit的末尾位置
	search_edit_->addAction(clear_action, QLineEdit::TrailingPosition);

	// 当需要显示清除图标时，更改为实际的清除图标
	connect(search_edit_, &QLineEdit::textChanged, [clear_action](const QString& text) {
		if (!text.isEmpty()) {
			clear_action->setIcon(QIcon("../res/close_search.png"));
		} else {
			clear_action->setIcon(
				QIcon("../res/close_transparent.png"));	 // 文本为空时，切换回透明图标
		}
	});

	// 连接清除动作的触发信号到槽函数，用于清除文本
	connect(clear_action, &QAction::triggered, [this, clear_action]() {
		search_edit_->clear();
		clear_action->setIcon(
			QIcon("../res/close_transparent.png"));	 // 清除文本后，切换回透明图标
		search_edit_->clearFocus();
		// 清除按钮被按下则不显示搜索框
		ShowSearch(false);
	});

	SetQssName();
	LoadStyleSheet(this, "../style/chat_window.qss");
}

void ChatWindow::SetQssName() {
	// add_friend_btn_->setObjectName("add_friend_btn");
	// apply_tab_label_->setObjectName("apply_tab_label");
	friend_tab_label_->setObjectName("friend_tab_label");
	session_tab_label_->setObjectName("session_tab_label");
}

// 初始化主窗口
void ChatWindow::InitMainWindow() {
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setSpacing(0);					 // 内部元素之间的间隔
	layout->setContentsMargins(0, 0, 0, 0);	 // 元素距离四个边界的距离
	setLayout(layout);

	window_left_ = new QWidget();
	window_mid_ = new QWidget();
	window_right_ = new QWidget();

	// widget 宽度
	window_left_->setFixedWidth(60);
	window_mid_->setFixedWidth(280);
	window_right_->setMinimumWidth(500);

	// widget 背景色
	window_left_->setStyleSheet("QWidget { background-color: rgb(208, 221, 247); }");
	window_mid_->setStyleSheet("QWidget { background-color: rgb(240, 243, 250); }");
	window_right_->setStyleSheet(
		"QWidget {"
		"    background-color: rgb(245, 245, 245);"
		"}");

	// 添加到主窗口
	layout->addWidget(window_left_);
	layout->addWidget(window_mid_);
	layout->addWidget(window_right_);
}

// 初始化选项栏
void ChatWindow::InitLeftWindow() {
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(20);
	layout->setContentsMargins(0, 10, 0, 0);
	window_left_->setLayout(layout);

	// 按钮样式
	static const QString buttonStyle =
		"QPushButton {"
		"    background-color: transparent;"
		"    border: none;"
		"}";

	// 用户头像
	user_avatar_ = new QPushButton();
	user_avatar_->setFixedSize(45, 45);
	user_avatar_->setIconSize(QSize(45, 45));
	user_avatar_->setIcon(QIcon("../res/head_1.jpg"));
	user_avatar_->setStyleSheet(buttonStyle);
	user_avatar_->setCursor(Qt::PointingHandCursor);
	layout->addWidget(user_avatar_, 1, Qt::AlignTop | Qt::AlignHCenter);

	// 会话添加标签页按钮
	session_tab_label_ = new StateWidget();
	session_tab_label_->setFixedSize(25, 25);
	// session_tab_label_->setPixmap(QPixmap("../res/chat_icon.png"));
	// session_tab_label_->setIcon(QIcon("../res/chat_icon.png"));
	session_tab_label_->setStyleSheet(buttonStyle);
	session_tab_label_->setCursor(Qt::PointingHandCursor);
	layout->addWidget(session_tab_label_, 1, Qt::AlignTop | Qt::AlignHCenter);

	// 好友添加标签页按钮
	// friendTabBtn = new QPushButton();
	// friendTabBtn->setFixedSize(25, 25);
	// friendTabBtn->setIconSize(QSize(25, 25));
	// friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
	// friendTabBtn->setStyleSheet(buttonStyle);
	// friendTabBtn->setCursor(Qt::PointingHandCursor);
	// layout->addWidget(friendTabBtn, 1, Qt::AlignTop | Qt::AlignHCenter);

	// 好友标签页按钮
	friend_tab_label_ = new StateWidget();
	friend_tab_label_->setFixedSize(25, 25);
	// friend_tab_label_->setIconSize(QSize(25, 25));
	// friend_tab_label_->setIcon(QIcon("../res/contact_list.png"));
	friend_tab_label_->setStyleSheet(buttonStyle);
	friend_tab_label_->setCursor(Qt::PointingHandCursor);
	layout->addWidget(friend_tab_label_, 1, Qt::AlignTop | Qt::AlignHCenter);
	layout->addStretch(20);
}

// 初始化会话栏
void ChatWindow::InitMidWindow() {
	QGridLayout* layout = new QGridLayout();
	layout->setContentsMargins(0, 20, 0, 0);
	layout->setHorizontalSpacing(0);
	layout->setVerticalSpacing(10);
	window_mid_->setLayout(layout);

	// 颜色变量
	const QString lightGray = "rgb(226, 226, 226)";
	const QString pressedGray = "rgb(209, 209, 209)";
	const QString placeholderGray = "rgb(150, 150, 150)";

	// 搜索框样式
	search_edit_ = new CustomizeEdit();
	search_edit_->setFixedHeight(30);
	search_edit_->setPlaceholderText("搜索");
	search_edit_->setStyleSheet(QString("QLineEdit {"
										"   border-radius: 5px;"
										"   background-color: %1;"
										"   padding: 0 10px;"
										"   font-size: 14px;"
										"   border: 1px solid transparent;"
										"}"
										"QLineEdit:focus {"
										"   border: 1px solid rgb(208, 221, 247);"
										"}"
										"QLineEdit::placeholder {"
										"   color: %2;"
										"}")
									.arg(lightGray, placeholderGray));

	// 添加好友按钮样式
	// add_friend_btn_ = new ClickedBtn();
	// add_friend_btn_->setFixedSize(30, 30);
	// add_friend_btn_->setIcon(QIcon("../res/add_friend_normal.png"));
	// add_friend_btn_->setIconSize(QSize(16, 16));
	// add_friend_btn_->setCursor(Qt::PointingHandCursor);
	// 搜索按钮样式
	search_btn_ = new ClickedBtn();
	search_btn_->setFixedSize(30, 30);
	search_btn_->setIcon(QIcon("../res/search.png"));
	search_btn_->setIconSize(QSize(16, 16));
	search_btn_->setCursor(Qt::PointingHandCursor);

	static const QString btnStyle =
		QString(
			"QPushButton {"
			"   border-radius: 5px;"
			"   background-color: %1;"
			"   border: none;"
			"}"
			"QPushButton:hover {"
			"   background-color: %2;"
			"}"
			"QPushButton:pressed {"
			"   background-color: %3;"
			"}")
			.arg(lightGray, pressedGray, "rgb(190, 190, 190)");

	// add_friend_btn_->setStyleSheet(btnStyle);
	search_btn_->setStyleSheet(btnStyle);

	chat_user_list_ = new ChatUserList();
	friend_list_ = new FriendList();
	search_list_ = new SearchList();

	// QSpacerItem 插入空隙
	layout->addItem(new QSpacerItem(10, 0), 0, 0);
	layout->addWidget(search_edit_, 0, 1);
	layout->addItem(new QSpacerItem(10, 0), 0, 2);
	layout->addWidget(search_btn_, 0, 3);
	layout->addItem(new QSpacerItem(10, 0), 0, 4);
	layout->addWidget(chat_user_list_, 1, 0, 1, 5);
	layout->addItem(new QSpacerItem(10, 0), 0, 6);
	layout->addWidget(friend_list_, 2, 0, 1, 5);
	layout->addItem(new QSpacerItem(10, 0), 0, 8);
	layout->addWidget(search_list_, 3, 0, 1, 5);
}

// 初始化消息栏
void ChatWindow::InitRightWindow() {
	// 右侧窗口布局管理器
	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->setSpacing(0);
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->setAlignment(Qt::AlignTop);
	window_right_->setLayout(vlayout);
	// 栈式容器
	stacked_widget_ = new QStackedWidget();
	vlayout->addWidget(stacked_widget_);
	// 子界面
	chat_widget_ = new ChatWidget();
	stacked_widget_->addWidget(chat_widget_);  // 索引0
	// 好友申请页面
	apply_friend_widget_ = new ApplyFriendWidget();
	stacked_widget_->addWidget(apply_friend_widget_);  // 索引1

	stacked_widget_->setCurrentWidget(chat_widget_);
}
