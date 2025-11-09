#include "user_mgr.h"

#include <QJsonArray>

UserMgr::UserMgr() : user_info_(nullptr), chat_loaded_(0), contact_loaded_(0) {}

UserMgr::~UserMgr() {}

void UserMgr::SetUserInfo(std::shared_ptr<UserInfo> user_info) { user_info_ = user_info; }

void UserMgr::SetToken(QString token) { token_ = token; }

QString UserMgr::GetUid() { return user_info_->uid_; }

QString UserMgr::GetName() { return user_info_->name_; }

QString UserMgr::GetBackName() { return user_info_->back_name_; }

QString UserMgr::GetIcon() { return user_info_->icon_; }

QString UserMgr::GetDesc() { return user_info_->desc_; }

std::shared_ptr<UserInfo> UserMgr::GetUserInfo() { return user_info_; }

void UserMgr::AppendApplyList(QJsonArray array) {
	// 遍历 QJsonArray 并输出每个元素
	for (const QJsonValue& value : array) {
		auto name = value["name"].toString();
		auto uid = value["uid"].toString();
		auto icon = value["icon"].toString();
		auto gender = value["gender"].toInt();
		auto desc = value["descs"].toString();
		// auto sex = value["sex"].toInt();
		auto status = value["status"].toInt();
		auto info =
			std::make_shared<ApplyInfo>(uid, name, desc, icon, "", gender, status);
		apply_list_.push_back(info);
	}
}

void UserMgr::AppendFriendList(QJsonArray array) {
	// 遍历 QJsonArray 并输出每个元素
	for (const QJsonValue& value : array) {
		auto name = value["username"].toString();
		// auto desc = value["desc"].toString();
		auto icon = value["icon"].toString();
		// auto nick = value["nick"].toString();
		auto sex = value["gender"].toInt();
		auto uid = value["user_id"].toString();
		auto back = value["back_name"].toString();

		auto info = std::make_shared<FriendInfo>(uid, name, "", icon, sex, "", back);
		friend_list_.push_back(info);
		friend_map_.insert(uid, info);
	}
}

bool UserMgr::CheckFriendById(QString uid) {
	auto iter = friend_map_.find(uid);
	if (iter == friend_map_.end()) {
		return false;
	}

	return true;
}

std::shared_ptr<FriendInfo> UserMgr::GetFriendById(QString uid) {
	auto find_it = friend_map_.find(uid);
	if (find_it == friend_map_.end()) {
		return nullptr;
	}

	return *find_it;
}

void UserMgr::AppendFriendChatMsg(QString friend_id,
								  std::vector<std::shared_ptr<TextChatData>> msgs) {
	auto find_iter = friend_map_.find(friend_id);
	if (find_iter == friend_map_.end()) {
		qDebug() << "append friend uid  " << friend_id << " not found";
		return;
	}

	find_iter.value()->AppendChatMsgs(msgs);
}

void UserMgr::AddFriend(std::shared_ptr<UserInfo> user_info) {
	auto friend_info = std::make_shared<FriendInfo>(
		user_info->uid_, user_info->name_, user_info->back_name_, user_info->icon_,
		user_info->gender, "", user_info->back_name_, "");
	friend_map_.insert(user_info->uid_, friend_info);
}

std::vector<std::shared_ptr<ApplyInfo>> UserMgr::GetApplyList() { return apply_list_; }

std::vector<std::shared_ptr<FriendInfo>> UserMgr::GetFriendList() { return friend_list_; }
