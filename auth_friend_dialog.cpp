#include "auth_friend_dialog.h"

#include <QVBoxLayout>
#include <QJsonObject>
#include "net/tcp_mgr.h"
#include "user_mgr.h"
#include <QJsonDocument>
#include <iostream>


AuthFriendDialog::AuthFriendDialog(QWidget* parent) : QDialog(parent) {
	SetUI();

	// 连接确认和取消按钮的槽函数
	connect(cancel_button_, &QPushButton::clicked, this, &AuthFriendDialog::close);
	connect(auth_button_, &QPushButton::clicked, this, &AuthFriendDialog::SlotAuthFriend);
}

void AuthFriendDialog::SlotAuthFriend() {
    // qDebug() << "Slot Apply Sure ";
    std::cout << "Slot Apply Sure " << std::endl;

    //添加发送逻辑
    QJsonObject jsonObj;
    auto uid = UserMgr::GetInstance()->GetUid();
    jsonObj["from_uid"] = uid;
    jsonObj["to_uid"] = apply_info_->uid_;
    QString back_name = auth_user_backname_lineedit_->text();
    jsonObj["back_name"] = back_name;

	auto hello = auth_user_hello_lineedit_->text();
	if (!hello.isEmpty()){
		jsonObj["descs"] = hello;
	}

    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    //发送tcp请求给chat server
    emit TcpMgr::GetInstance()->SigSendData(ReqId::AUTH_FRIEND_REQ, jsonData);

    this->close();
}

void AuthFriendDialog::SetApplyInfo(std::shared_ptr<ApplyInfo> apply_info) {
	apply_info_ = apply_info;
	auth_user_name_lineedit_->setText(apply_info_->name_);
    // 设置只读
	auth_user_name_lineedit_->setReadOnly(true);
}

void AuthFriendDialog::SetUI() {
	setFixedSize(361, 621);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(20, 20, 20, 20);
	layout->setSpacing(10);

	// 标题
	title_label_ = new QLabel(this);
	title_label_->setText("好友认证");
	title_label_->setAlignment(Qt::AlignCenter);
	title_label_->setStyleSheet("font-size: 20px; font-weight: bold;");
	layout->addWidget(title_label_);

	QVBoxLayout* content_widget_ = new QVBoxLayout();

	auth_user_name_label_ = new QLabel();
	auth_user_name_label_->setText("用户名:");
	auth_user_name_label_->setGeometry(20, 20, 100, 30);
	auth_user_name_label_->setStyleSheet("font-size: 14px;");

	auth_user_name_lineedit_ = new QLineEdit();
	auth_user_name_lineedit_->setGeometry(120, 20, 160, 30);
	auth_user_name_lineedit_->setStyleSheet("font-size: 14px;");

	auth_user_backname_label_ = new QLabel();
	auth_user_backname_label_->setText("备注:");
	auth_user_backname_label_->setGeometry(20, 60, 100, 30);
	auth_user_backname_label_->setStyleSheet("font-size: 14px;");

	auth_user_backname_lineedit_ = new QLineEdit();
	auth_user_backname_lineedit_->setGeometry(120, 60, 160, 30);
	auth_user_backname_lineedit_->setStyleSheet("font-size: 14px;");

	// 招呼语
	auth_user_hello_label_ = new QLabel();
	auth_user_hello_label_->setText("招呼语:");
	auth_user_hello_label_->setGeometry(20, 100, 100, 30);
	auth_user_hello_label_->setStyleSheet("font-size: 14px;");

	auth_user_hello_lineedit_ = new QLineEdit();
	auth_user_hello_lineedit_->setGeometry(120, 100, 160, 30);
	auth_user_hello_lineedit_->setStyleSheet("font-size: 14px;");

	content_widget_->addWidget(auth_user_name_label_);
	content_widget_->addWidget(auth_user_name_lineedit_);
	content_widget_->addWidget(auth_user_backname_label_);
	content_widget_->addWidget(auth_user_backname_lineedit_);
	content_widget_->addWidget(auth_user_hello_label_);
	content_widget_->addWidget(auth_user_hello_lineedit_);

	layout->addLayout(content_widget_);

	layout->addSpacerItem(
		new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

	QHBoxLayout* button_layout = new QHBoxLayout();
	auth_button_ = new QPushButton();
	auth_button_->setText("确认");
	auth_button_->setGeometry(20, 120, 80, 30);
	auth_button_->setStyleSheet("font-size: 14px;");
	button_layout->addWidget(auth_button_);

	cancel_button_ = new QPushButton();
	cancel_button_->setText("取消");
	cancel_button_->setGeometry(120, 120, 80, 30);
	cancel_button_->setStyleSheet("font-size: 14px;");
	button_layout->addWidget(cancel_button_);
	layout->addLayout(button_layout);
}