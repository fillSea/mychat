#ifndef USER_DATA_H
#define USER_DATA_H
#include <QString>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

struct TextChatData;

struct UserInfo {
	UserInfo(QString uid, QString name, QString back_name, QString icon, int sex,
			 QString last_msg = "", QString desc = "")
		: uid_(uid),
		  name_(name),
		  back_name_(back_name),
		  icon_(icon),
		  gender(sex),
		  last_msg_(last_msg),
		  desc_(desc) {}

	// UserInfo(std::shared_ptr<AuthInfo> auth):
	//     uid_(auth->uid_),name_(auth->name_),back_name(auth->back_name),
	//     icon_(auth->icon_),gender(auth->gender),last_msg_(""),desc_(""){}

	UserInfo(QString uid, QString name, QString icon)
		: uid_(uid),
		  name_(name),
		  icon_(icon),
		  back_name_(name_),
		  gender(0),
		  last_msg_(""),
		  desc_("") {}

	// UserInfo(std::shared_ptr<AuthRsp> auth):
	//     uid_(auth->uid_),name_(auth->name_),back_name(auth->back_name),
	//     icon_(auth->icon_),gender(auth->gender),last_msg_(""){}

	// UserInfo(std::shared_ptr<SearchInfo> search_info):
	//     uid_(search_info->uid_),name_(search_info->name_),back_name(search_info->back_name),
	// icon_(search_info->icon_),gender(search_info->gender),last_msg_(""){

	// }

	// UserInfo(std::shared_ptr<FriendInfo> friend_info):
	//     uid_(friend_info->uid_),name_(friend_info->name_),back_name(friend_info->back_name),
	//     icon_(friend_info->icon_),gender(friend_info->gender),last_msg_(""){
	//         chat_msgs = friend_info->chat_msgs;
	//     }

	QString uid_;
	QString name_;
	QString back_name_;
	QString icon_;
	int gender;
	QString desc_;
	QString last_msg_;
	std::vector<std::shared_ptr<TextChatData>> chat_msgs_;
};

class SearchInfo {
public:
	SearchInfo(QString uid, QString name, QString email, int gender, QString icon,
			   QString nick, QString desc)
		: uid_(uid), name_(name), email_(email), gender_(gender), icon_(icon) {}
	QString uid_;
	QString name_;
	// QString nick;
	QString email_;
	int gender_;
	QString icon_;
	// QString desc_;
};

struct ApplyInfo {
	ApplyInfo(QString uid, QString name, QString desc, QString icon, QString nick,
			  int sex, int status)
		: uid_(uid),
		  name_(name),
		  desc_(desc),
		  icon_(icon),
		  back_name_(nick),
		  gender_(sex),
		  status_(status) {}

	// ApplyInfo(std::shared_ptr<AddFriendApply> addinfo)
	//     :uid_(addinfo->from_uid_),name_(addinfo->name_),
	//       desc_(addinfo->desc_),icon_(addinfo->icon_),
	//       back_name_(addinfo->back_name_),gender_(addinfo->gender_),
	//       status_(0)
	// {}
	void SetIcon(QString head) { icon_ = head; }
	QString uid_;
	QString name_;
	QString desc_;
	QString icon_;
	QString back_name_;
	int gender_;
	int status_;
};

// 好友认证响应
struct AuthRsp {
	AuthRsp(QString peer_uid, QString peer_name, QString peer_nick, QString peer_icon,
			int peer_sex)
		: uid_(peer_uid),
		  name_(peer_name),
		  nick_(peer_nick),
		  icon_(peer_icon),
		  gender_(peer_sex) {}

	QString uid_;
	QString name_;
	QString nick_;
	QString icon_;
	int gender_;
};

// 好友信息
struct FriendInfo {
	FriendInfo(QString uid, QString name, QString nick, QString icon, int sex,
			   QString desc, QString back, QString last_msg = "")
		: uid_(uid),
		  name_(name),
		  nick_(nick),
		  icon_(icon),
		  gender_(sex),
		  desc_(desc),
		  back_name_(back),
		  last_msg_(last_msg) {}

	// FriendInfo(std::shared_ptr<AuthInfo> auth_info):uid_(auth_info->uid_),
	// nick_(auth_info->nick_),icon_(auth_info->icon_),name_(auth_info->name_),
	//   gender_(auth_info->gender_){}

	FriendInfo(std::shared_ptr<AuthRsp> auth_rsp)
		: uid_(auth_rsp->uid_),
		  nick_(auth_rsp->nick_),
		  icon_(auth_rsp->icon_),
		  name_(auth_rsp->name_),
		  gender_(auth_rsp->gender_) {}

	void AppendChatMsgs(const std::vector<std::shared_ptr<TextChatData>> text_vec);

	QString uid_;
	QString name_;
	QString nick_;
	QString icon_;
	int gender_;
	QString desc_;
	QString back_name_;
	QString last_msg_;
	std::vector<std::shared_ptr<TextChatData>> chat_msgs_;
};

struct TextChatData {
	TextChatData(QString msg_id, QString msg_content, QString fromuid, QString touid)
		: msg_id_(msg_id),
		  msg_content_(msg_content),
		  from_uid_(fromuid),
		  to_uid_(touid) {}
	QString msg_id_;
	QString msg_content_;
	QString from_uid_;
	QString to_uid_;
};

struct TextChatMsg{
    TextChatMsg(QString fromuid, QString touid, QJsonArray arrays):
        from_uid_(fromuid),to_uid_(touid){
        for(auto  msg_data : arrays){
            auto msg_obj = msg_data.toObject();
            auto content = msg_obj["content"].toString();
            auto msgid = msg_obj["unique_id"].toString();
            auto msg_ptr = std::make_shared<TextChatData>(msgid, content,fromuid, touid);
            chat_msgs_.push_back(msg_ptr);
        }
    }
    QString to_uid_;
    QString from_uid_;
    std::vector<std::shared_ptr<TextChatData>> chat_msgs_;
};

#endif	// USER_DATA_H