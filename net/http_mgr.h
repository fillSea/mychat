#ifndef HTTPMGR_H
#define HTTPMGR_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QWidget>

#include "../const.h"
#include "../utils/singleton.h"


// http请求管理类
class HttpMgr : public QWidget,
				public Singleton<HttpMgr>,
				public std::enable_shared_from_this<HttpMgr> {
	Q_OBJECT
	friend class Singleton<HttpMgr>;

public:
	~HttpMgr(){};
	// 发送http post 请求
	void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

private:
	friend class Singleton<HttpMgr>;
	HttpMgr();
signals:
	// http发送完成信号
	void SigHttpFinish(ReqId id, QString res, ErrorCodes err, Modules mod);
	// http 注册响应信号
	void SigRegModFinish(ReqId id, QString res, ErrorCodes err);
	// http 登录响应信号
	void SigLoginModFinish(ReqId id, QString res, ErrorCodes err);
private slots:
	// http请求完成槽函数
	void SlotHttpFinish(ReqId id, QString res, ErrorCodes err, Modules mod);
private:
	// 负责发送和接收网络请求
	QNetworkAccessManager manager_;
};

#endif	// HTTPMGR_H