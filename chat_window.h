#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <qpushbutton.h>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#include "apply_friend_widget.h"
#include "chat_user_list.h"
#include "chat_widget.h"
#include "const.h"
#include "friend_list.h"
#include "search_list.h"
#include "user_data.h"
#include "utils/clicked_btn.h"
#include "utils/customize_edit.h"
#include "utils/state_widget.h"

// TODO:加载好友申请
// TODO:好友认证
class ChatWindow : public QWidget {
	Q_OBJECT
public:
	explicit ChatWindow(QWidget* parent = nullptr);
	~ChatWindow();

private:
	void SetUI();
	void SetQssName();
	// 初始化主窗口
	void InitMainWindow();
	// 初始化选项栏
	void InitLeftWindow();
	// 初始化会话栏
	void InitMidWindow();
	// 初始化消息栏
	void InitRightWindow();
	// 显示界面
	void ShowSearch(bool bsearch);
	// 测试数据, 模拟加载聊天列表
	void addChatUserList();
	// 加载更多聊天用户
	void LoadMoreChatUser();
	// 加载更多搜索用户
	void LoadMoreSearchUser();
	// 清除所有标签状态
	void ClearLabelState(StateWidget* lb);
	// 添加标签组
	void AddLBGroup(StateWidget* lb);
	// 测试数据, 模拟加载搜索用户列表
	void addSearchUserList();
	void addFriendList();
	// 更新聊天消息
	void UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>> msgdata);
private slots:
	// 切换到会话标签页
	void SlotSwitchTabChat();
	// 切换到好友界面
	void SlotSwitchTabFriend();
	// 搜索框输入变化
	void SlotTextChanged(const QString& text);
	// 搜索按钮点击事件
	void SlotSearch();
	// 搜索完成信号槽
	void SlotGetSearchUsers(QVector<std::shared_ptr<SearchInfo>>& users);
	// 搜索完成信号槽
	void SlotGetSearchUser(std::shared_ptr<SearchInfo> user);
	// 动态加载聊天列表
	void SlotLoadingChatUser();
	// 动态加载搜索用户列表
	void SlotLoadingSearchUser();
	void SlotLoadingFriend();
	// 切换到好友申请界面
	void SlotSwitchApplyFriendWidget();
	// 切换到聊天窗口
	void SlotSwitchChat(std::shared_ptr<UserInfo> user_info);
	// 接收对端的消息
	void SlotTextChatMsg(std::shared_ptr<TextChatMsg> msg);

private:
	bool b_loading_;	// 是否正在加载中
	ChatUIMode mode_;	// 聊天窗口模式
	ChatUIMode state_;	// 聊天窗口状态
	QList<StateWidget*> label_list_;
	QMap<QString, QListWidgetItem*> chat_items_added_;

private:
	// 左中右三个窗口
	QWidget* window_left_;
	QWidget* window_mid_;
	QWidget* window_right_;

	// ========== 左侧 ==========
	QPushButton* user_avatar_;	// 用户头像
	// StateWidget* session_tab_label_;	// 会话标签页按钮
	StateWidget* session_tab_label_;
	StateWidget* friend_tab_label_;	 // 好友标签页按钮
	// StateWidget* apply_tab_label_;	// 好友申请标签页按钮

	// ========== 中间 ==========
	CustomizeEdit* search_edit_;  // 用户搜索框
	// ClickedBtn* add_friend_btn_;	// 添加好友按钮
	ClickedBtn* search_btn_;		// 搜索按钮
	ChatUserList* chat_user_list_;	// 聊天列表
	SearchList* search_list_;		// 搜索结果列表
	FriendList* friend_list_;		// 好友列表

	// ========== 右侧 ==========
	QStackedWidget* stacked_widget_;		  // 栈式窗口容器
	ChatWidget* chat_widget_;				  // 聊天窗口
	ApplyFriendWidget* apply_friend_widget_;  // 好友申请页面
};

#endif	// CHAT_WINDOW_H
