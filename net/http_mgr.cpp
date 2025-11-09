#include "http_mgr.h"
#include "../const.h"
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>


HttpMgr::HttpMgr(){
    //连接http请求和完成信号，信号槽机制保证队列消费
    connect(this, &HttpMgr::SigHttpFinish, this, &HttpMgr::SlotHttpFinish);
}

// 发送http post 请求
void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod){
    // 创建一个http post请求，并设置请求头和请求体
    QByteArray data = QJsonDocument(json).toJson();
    // 通过url构造请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    // 发送请求，并处理响应，获取自己的智能指针，构造伪闭包并增加智能指针引用计数
    auto self = shared_from_this();
    // 发送 post 请求
    QNetworkReply* reply = manager_.post(request, data);
    // 设置信号和槽等待发送完成
    QObject::connect(reply, &QNetworkReply::finished, [reply, self, req_id, mod](){
        // 请求完成, 处理响应
        // 处理错误情况
        if (reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            // 发送信号通知完成
            emit self->SigHttpFinish(req_id, "", ErrorCodes::ERRNETWORK, mod);
            // 回收reply
            reply->deleteLater();
            return;
        }
        
        // 无错误则读取响应
        QString res = reply->readAll();
        // 发送信号通知完成
        emit self->SigHttpFinish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

// http请求完成槽函数
void HttpMgr::SlotHttpFinish(ReqId id, QString res, ErrorCodes err, Modules mod){
    if(mod == Modules::REGISTERMOD){
        //发送信号通知指定模块http响应结束
        emit SigRegModFinish(id, res, err);
    }

    if(mod == Modules::RESETMOD){
        //发送信号通知指定模块http响应结束
        // emit sig_reset_mod_finish(id, res, err);
    }

    if(mod == Modules::LOGINMOD){
        emit SigLoginModFinish(id, res, err);
    }
}