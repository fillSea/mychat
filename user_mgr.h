#ifndef USERMGR_H
#define USERMGR_H
#include <QObject>
#include <memory>

#include "user_data.h"
#include "utils/singleton.h"
#include <QMap>

// 用户数据管理类
class UserMgr : public QObject,
				public Singleton<UserMgr>,
				public std::enable_shared_from_this<UserMgr> {
	Q_OBJECT
public:
	friend class Singleton<UserMgr>;
	~UserMgr();
	void SetUserInfo(std::shared_ptr<UserInfo> user_info);
	void SetToken(QString token);
	QString GetUid();
	QString GetName();
	QString GetBackName();
	QString GetIcon();
	QString GetDesc();
	std::shared_ptr<UserInfo> GetUserInfo();
	void AppendApplyList(QJsonArray array);
	void AppendFriendList(QJsonArray array);
	std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
	std::vector<std::shared_ptr<FriendInfo>> GetFriendList();
	// void AddApplyList(std::shared_ptr<ApplyInfo> app);
	// bool AlreadyApply(int uid);
	// std::vector<std::shared_ptr<FriendInfo>> GetChatListPerPage();
	// bool IsLoadChatFin();
	// void UpdateChatLoadedCount();
	// std::vector<std::shared_ptr<FriendInfo>> GetConListPerPage();
	// void UpdateContactLoadedCount();
	// bool IsLoadConFin();
	bool CheckFriendById(QString uid);
	// void AddFriend(std::shared_ptr<AuthRsp> auth_rsp);
	// void AddFriend(std::shared_ptr<AuthInfo> auth_info);
	void AddFriend(std::shared_ptr<UserInfo> user_info);
	std::shared_ptr<FriendInfo> GetFriendById(QString uid);
	void AppendFriendChatMsg(QString friend_id, std::vector<std::shared_ptr<TextChatData>>);

private:
	UserMgr();
private:
	std::shared_ptr<UserInfo> user_info_;//用户信息
	std::vector<std::shared_ptr<ApplyInfo>> apply_list_;
	std::vector<std::shared_ptr<FriendInfo>> friend_list_;
	// 好友映射表，key为好友uid，value为好友信息
	QMap<QString, std::shared_ptr<FriendInfo>> friend_map_;
	QString token_;//用户token
	int chat_loaded_;
	int contact_loaded_;

public slots:
	// void SlotAddFriendRsp(std::shared_ptr<AuthRsp> rsp);
	// void SlotAddFriendAuth(std::shared_ptr<AuthInfo> auth);
};

#endif	// USERMGR_H
