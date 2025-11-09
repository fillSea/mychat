#include "tcp_mgr.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include <functional>
#include <iostream>
#include <memory>

#include "../user_data.h"
#include "../user_mgr.h"


TcpMgr::TcpMgr()
	: host_(""), port_(0), b_recv_pending_(false), message_id_(0), message_len_(0) {
	InitHandlers();
	// 连接成功信号槽连接
	QObject::connect(&socket_, &QTcpSocket::connected, [&]() {
		std::cout << "Connected to ChatServer" << std::endl;
		qDebug() << "Connected to server!";
		// 连接建立后发送信号
		emit SigConnectSuccess(true);
	});

	// 读取数据
	QObject::connect(&socket_, &QTcpSocket::readyRead, [&]() {
		// 当有数据可读时，读取所有数据
		// 读取所有数据并追加到缓冲区
		buffer_.append(socket_.readAll());

		QDataStream stream(&buffer_, QIODevice::ReadOnly);
		stream.setVersion(QDataStream::Qt_6_0);

		forever {
			// 先解析头部
			if (!b_recv_pending_) {
				// 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）
				if (buffer_.size() < static_cast<int>(sizeof(quint16) * 2)) {
					return;	 // 数据不够，等待更多数据
				}

				// 预读取消息ID和消息长度，但不从缓冲区中移除
				stream >> message_id_ >> message_len_;

				// 将buffer 中的前四个字节移除
				buffer_ = buffer_.mid(sizeof(quint16) * 2);

				// 输出读取的数据
				std::cout << "Message ID:" << message_id_ << ", Length:" << message_len_
						  << std::endl;
				qDebug() << "Message ID:" << message_id_ << ", Length:" << message_len_;
			}

			// buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
			if (buffer_.size() < message_len_) {
				b_recv_pending_ = true;
				return;
			}

			b_recv_pending_ = false;
			// 读取消息体
			QByteArray messageBody = buffer_.mid(0, message_len_);
			qDebug() << "receive body msg is " << messageBody;

			buffer_ = buffer_.mid(message_len_);
			HandleMsg(ReqId(message_id_), message_len_, messageBody);
		}
	});

	// 5.15 之后版本
	QObject::connect(
		&socket_, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
		[&](QAbstractSocket::SocketError socketError) {
			Q_UNUSED(socketError)
			qDebug() << "Error:" << socket_.errorString();
		});

	// 处理连接断开
	QObject::connect(&socket_, &QTcpSocket::disconnected, [&]() {
		qDebug() << "Disconnected from server.";
		// 并且发送通知到界面
		// emit sig_connection_closed();
	});
	// 连接发送信号用来发送数据
	QObject::connect(this, &TcpMgr::SigSendData, this, &TcpMgr::SlotSendData);
}

// 注册处理函数
void TcpMgr::InitHandlers() {
	handlers_.insert(ReqId::CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data) {
		Q_UNUSED(len);
		qDebug() << "handle id is " << static_cast<int>(id);
		std::cout << "CHAT_LOGIN_RSP" << std::endl;
		// 将QByteArray转换为QJsonDocument
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

		// 检查转换是否成功
		if (jsonDoc.isNull()) {
			qDebug() << "Failed to create QJsonDocument.";
			std::cout << "Failed to create QJsonDocument." << std::endl;
			return;
		}

		QJsonObject jsonObj = jsonDoc.object();
		qDebug() << "data jsonobj is " << jsonObj;

		if (!jsonObj.contains("error")) {
			int err = static_cast<int>(ErrorCodes::ERRJSON);
			qDebug() << "Login Failed, err is Json Parse Err" << err;
			emit SigLoginFailed(err);
			return;
		}

		int err = jsonObj["error"].toInt();
		if (err != static_cast<int>(ErrorCodes::SUCCESS)) {
			qDebug() << "Login Failed, err is " << err;
			emit SigLoginFailed(err);
			return;
		}

		auto uid = jsonObj["user_id"].toString();
		auto name = jsonObj["username"].toString();
		auto icon = jsonObj["icon"].toString();
		auto gender = jsonObj["gender"].toInt();
		auto user_info = std::make_shared<UserInfo>(uid, name, "", icon, gender, "", "");

		// 设置用户信息
		UserMgr::GetInstance()->SetUserInfo(user_info);
		UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
		// 好友申请列表
		if(jsonObj.contains("apply_list")){
			std::cout << "apply_list size: " << jsonObj["apply_list"].toArray().size() << std::endl;
		    UserMgr::GetInstance()->AppendApplyList(jsonObj["apply_list"].toArray());
		}

		//添加好友列表
		if (jsonObj.contains("friend_list")) {
		    UserMgr::GetInstance()->AppendFriendList(jsonObj["friend_list"].toArray());
		}

		emit SigSwichChatWindow();
	});

	handlers_.insert(ReqId::SEARCH_USER_RSP,
					 std::bind(&TcpMgr::HandleSearchUserRsp, this, std::placeholders::_1,
							   std::placeholders::_2, std::placeholders::_3));
	handlers_.insert(ReqId::AUTH_FRIEND_RSP,
					 std::bind(&TcpMgr::HandleAuthFriendRsp, this, std::placeholders::_1,
							   std::placeholders::_2, std::placeholders::_3));
	handlers_.insert(ReqId::TEXT_CHAT_MSG_RSP,
					 std::bind(&TcpMgr::HandleTextChatMsgRsp, this, std::placeholders::_1,
							   std::placeholders::_2, std::placeholders::_3));
	handlers_.insert(ReqId::NOTIFY_TEXT_CHAT_MSG_REQ,
					 std::bind(&TcpMgr::HandleTextChatMsgNotify, this, std::placeholders::_1,
							   std::placeholders::_2, std::placeholders::_3));
}

// 发送消息, 服务器给本机回包
void TcpMgr::HandleTextChatMsgRsp(ReqId id, int len, QByteArray data){
	Q_UNUSED(len);
    qDebug() << "handle id is " << static_cast<int>(id) << " data is " << data;
    // 将QByteArray转换为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    // 检查转换是否成功
    if (jsonDoc.isNull()) {
        qDebug() << "Failed to create QJsonDocument.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if (!jsonObj.contains("error")) {
        int err = static_cast<int>(ErrorCodes::ERRJSON);
        qDebug() << "Chat Msg Rsp Failed, err is Json Parse Err" << err;
        return;
    }

    int err = jsonObj["error"].toInt();
    if (err != static_cast<int>(ErrorCodes::SUCCESS)) {
        qDebug() << "Chat Msg Rsp Failed, err is " << err;
        return;
    }

    qDebug() << "Receive Text Chat Rsp Success";
     std::cout << "Receive Text Chat Rsp Success" << std::endl;

}

// 发送消息, 接收方回包
void TcpMgr::HandleTextChatMsgNotify(ReqId id, int len, QByteArray data){
	Q_UNUSED(len);
    qDebug() << "handle id is " << static_cast<int>(id) << " data is " << data;
    // 将QByteArray转换为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    // 检查转换是否成功
    if (jsonDoc.isNull()) {
        qDebug() << "Failed to create QJsonDocument.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if (!jsonObj.contains("error")) {
        int err = static_cast<int>(ErrorCodes::ERRJSON);
        qDebug() << "Notify Chat Msg Failed, err is Json Parse Err" << err;
        return;
    }

    int err = jsonObj["error"].toInt();
    if (err != static_cast<int>(ErrorCodes::SUCCESS)) {
        qDebug() << "Notify Chat Msg Failed, err is " << err;
        return;
    }

	qDebug() << "Receive Text Chat Notify Success" ;
	auto msg_ptr = std::make_shared<TextChatMsg>(jsonObj["from_uid"].toString(),
			jsonObj["to_uid"].toString(),jsonObj["text_array"].toArray());
    emit SigTextChatMsg(msg_ptr);
}

// 处理搜索用户请求
void TcpMgr::HandleSearchUserRsp(ReqId id, int len, QByteArray data) {
	Q_UNUSED(len);
	qDebug() << "handle id is " << static_cast<int>(id);
	std::cout << "SEARCH_USER_RSP" << std::endl;
	// 将QByteArray转换为QJsonDocument
	QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

	// 检查转换是否成功
	if (jsonDoc.isNull()) {
		qDebug() << "Failed to create QJsonDocument.";
		return;
	}

	QJsonObject jsonObj = jsonDoc.object();
	qDebug() << "data jsonobj is " << jsonObj;

	if (!jsonObj.contains("error")) {
		int err = static_cast<int>(ErrorCodes::ERRJSON);
		qDebug() << "Login Failed, err is Json Parse Err" << err;
		emit SigLoginFailed(err);
		return;
	}

	int err = jsonObj["error"].toInt();
	if (err != static_cast<int>(ErrorCodes::SUCCESS)) {
		qDebug() << "Login Failed, err is " << err;
		emit SigLoginFailed(err);
		return;
	}

	// 判断是 user_id 还是 usename 查询
	if (jsonObj.contains("user_list")) {
		// username 查询
		auto user_list = jsonObj["user_list"].toArray();
		QVector<std::shared_ptr<SearchInfo>> users;
		for (auto user : user_list) {
			// 先将 QJsonValue 转换为 QJsonObject
			QJsonObject userObj = user.toObject();
			auto uid = userObj["user_id"].toString();
			auto name = userObj["username"].toString();
			auto icon = userObj["icon"].toString();
			auto gender = userObj["gender"].toInt();
			auto email = userObj["email"].toString();
			// 存储用户信息
			auto user_info =
				std::make_shared<SearchInfo>(uid, name, email, gender, icon, "", "");
			users.append(user_info);
		}
		emit SigGetSearchUsers(users);
	} else {
		// user_id 查询
		auto uid = jsonObj["user_id"].toString();
		auto name = jsonObj["username"].toString();
		auto icon = jsonObj["icon"].toString();
		auto gender = jsonObj["gender"].toInt();
		auto email = jsonObj["email"].toString();
		auto user_info =
			std::make_shared<SearchInfo>(uid, name, email, gender, icon, "", "");
		emit SigGetSearchUser(user_info);
	}
}

// 处理好友认证响应
void TcpMgr::HandleAuthFriendRsp(ReqId id, int len, QByteArray data){
	Q_UNUSED(len);
        qDebug() << "handle id is " << static_cast<int>(id) << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = static_cast<int>(ErrorCodes::ERRJSON);
            qDebug() << "Auth Friend Failed, err is Json Parse Err" << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != static_cast<int>(ErrorCodes::SUCCESS)) {
            qDebug() << "Auth Friend Failed, err is " << err;
            return;
        }

        auto name = jsonObj["username"].toString();
        // auto nick = jsonObj["back_name"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["gender"].toInt();
        auto uid = jsonObj["user_id"].toString();
        auto rsp = std::make_shared<AuthRsp>(uid, name, "", icon, sex);
        emit SigAuthRsp(rsp);

        qDebug() << "Auth Friend Success";
		std::cout << "Auth Friend Success" << std::endl;
}

// 连接tcp服务器
void TcpMgr::SlotTcpConnect(ServerInfo si) {
	qDebug() << "receive tcp connect signal";
	// 尝试连接到服务器
	qDebug() << "Connecting to server...";
	host_ = si.Host;
	port_ = static_cast<uint16_t>(si.Port.toUInt());
	// 连接服务器
	socket_.connectToHost(host_, port_);
}

// 发送数据信号
void TcpMgr::SlotSendData(ReqId reqId, QByteArray data) {
	uint16_t id = static_cast<uint16_t>(reqId);

	// 计算长度（使用网络字节序转换）
	quint16 len = static_cast<quint16>(data.length());

	// 创建一个QByteArray用于存储要发送的所有数据
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);

	// 设置数据流使用网络字节序
	out.setByteOrder(QDataStream::BigEndian);

	// 写入ID和长度
	out << id << len;

	// 添加字符串数据
	block.append(data);

	// 发送数据
	socket_.write(block);
	qDebug() << "tcp mgr send byte data is " << block;
}

void TcpMgr::HandleMsg(ReqId id, int len, QByteArray data) {
	auto find_iter = handlers_.find(id);
	if (find_iter == handlers_.end()) {
		qDebug() << "not found id [" << static_cast<int>(id) << "] to handle";
		return;
	}

	find_iter.value()(id, len, data);
}
