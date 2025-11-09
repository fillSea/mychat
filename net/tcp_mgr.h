#ifndef TCPMGR_H
#define TCPMGR_H
#include <QJsonArray>
#include <QObject>
#include <QTcpSocket>
#include "../const.h"
#include "../utils/singleton.h"
#include "../user_data.h"

class TcpMgr : public QObject,
			   public Singleton<TcpMgr>,
			   public std::enable_shared_from_this<TcpMgr> {
	Q_OBJECT
public:
	~TcpMgr() {}
	// void CloseConnection();
private:
	friend class Singleton<TcpMgr>;
	TcpMgr();
	// 注册处理函数
	void InitHandlers();
	// 处理数据
	void HandleMsg(ReqId id, int len, QByteArray data);
	// 处理搜索用户响应
	void HandleSearchUserRsp(ReqId id, int len, QByteArray data);
	// 处理好友认证响应
	void HandleAuthFriendRsp(ReqId id, int len, QByteArray data);
	// 发送消息, 发送方回包
	void HandleTextChatMsgRsp(ReqId id, int len, QByteArray data);
	// 发送消息, 接收方回包
	void HandleTextChatMsgNotify(ReqId id, int len, QByteArray data);
public slots:
	// 连接tcp服务器
	void SlotTcpConnect(ServerInfo);
	// 发送数据
	void SlotSendData(ReqId reqId, QByteArray data);
	
signals:
	// 连接成功信号
	void SigConnectSuccess(bool bsuccess);
	// 发送数据信号
	void SigSendData(ReqId reqId, QByteArray data);
	// 切换聊天对话框信号
	void SigSwichChatWindow();
	// void sig_load_apply_list(QJsonArray json_array);
	// 登录失败
	void SigLoginFailed(int);
	void SigGetSearchUsers(QVector<std::shared_ptr<SearchInfo>>&);
	void SigGetSearchUser(std::shared_ptr<SearchInfo>);
	// 好友认证响应信号
	void SigAuthRsp(std::shared_ptr<AuthRsp>);
	// void sig_user_search(std::shared_ptr<SearchInfo>);
	// void sig_friend_apply(std::shared_ptr<AddFriendApply>);
	// void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
	// void sig_auth_rsp(std::shared_ptr<AuthRsp>);
	void SigTextChatMsg(std::shared_ptr<TextChatMsg> msg);
	// void sig_notify_offline();
	// void sig_connection_closed();
private:
	QTcpSocket socket_;	   // 管理与远程主机的 TCP 连接
	QString host_;		   // 连接的主机地址
	uint16_t port_;		   // 连接的端口号
	QByteArray buffer_;	   // 用于存储接收到的数据
	bool b_recv_pending_;  // 标志位，表示是否还有数据等待接收（true-未收全, false-收全）
	quint16 message_id_;   // 消息 ID
	quint16 message_len_;  // 消息长度
	QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> handlers_;
};
#endif	// TCPMGR_H
