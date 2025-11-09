#include "apply_friend_dialog.h"

#include <QJsonObject>
#include <QVBoxLayout>
#include <QWidget>
#include "net/tcp_mgr.h"
#include "user_mgr.h"
#include <QJsonDocument>

//TODO:当用户未输入某些信息时，提示用户输入
//TODO:申请了好友成功后, 将添加好友按钮进行变化
ApplyFriendDialog::ApplyFriendDialog(QWidget* parent) : QDialog(parent) {
	SetUI();

	connect(cancel_button_, &QPushButton::clicked, this, &ApplyFriendDialog::close);
	// 发送好友申请按钮
	connect(apply_button_, &QPushButton::clicked, this,
			&ApplyFriendDialog::SlotApplyFriend);
}

ApplyFriendDialog::~ApplyFriendDialog() {}

void ApplyFriendDialog::SlotApplyFriend() {
	if (search_info_) {
		// 发送好友申请
		QJsonObject json_obj;

		auto uid = UserMgr::GetInstance()->GetUid();
		json_obj["user_id"] = uid;
		json_obj["apply_name"] = UserMgr::GetInstance()->GetName();

		auto back_name = apply_user_backname_lineedit_->text();
		if (back_name.isEmpty()) {
			back_name = apply_user_backname_lineedit_->placeholderText();
		}

		json_obj["back_name"] = back_name;
		json_obj["to_uid"] = search_info_->uid_;

        auto hello = apply_user_hello_lineedit_->text();
		if (!hello.isEmpty()) {
            json_obj["descs"] = hello;
		}


		QJsonDocument doc(json_obj);
		QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

		// 发送tcp请求给chat server
		emit TcpMgr::GetInstance()->SigSendData(ReqId::ADD_FRIEND_REQ, jsonData);

		close();
	}
}

void ApplyFriendDialog::SetInfo(std::shared_ptr<SearchInfo> info) {
	search_info_ = info;

	// 设置用户名
	apply_user_name_lineedit_->setText(search_info_->name_);
	// 设置只读
	apply_user_name_lineedit_->setReadOnly(true);
}

void ApplyFriendDialog::SetUI() {
	setFixedSize(361, 621);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(20, 20, 20, 20);
	layout->setSpacing(10);

	// 标题
	title_label_ = new QLabel(this);
	title_label_->setText("好友申请");
	title_label_->setAlignment(Qt::AlignCenter);
	title_label_->setStyleSheet("font-size: 20px; font-weight: bold;");
	layout->addWidget(title_label_);

	QVBoxLayout* content_widget_ = new QVBoxLayout();

	apply_user_name_label_ = new QLabel();
	apply_user_name_label_->setText("用户名:");
	apply_user_name_label_->setGeometry(20, 20, 100, 30);
	apply_user_name_label_->setStyleSheet("font-size: 14px;");

	apply_user_name_lineedit_ = new QLineEdit();
	apply_user_name_lineedit_->setGeometry(120, 20, 160, 30);
	apply_user_name_lineedit_->setStyleSheet("font-size: 14px;");

	apply_user_backname_label_ = new QLabel();
	apply_user_backname_label_->setText("备注:");
	apply_user_backname_label_->setGeometry(20, 60, 100, 30);
	apply_user_backname_label_->setStyleSheet("font-size: 14px;");

	apply_user_backname_lineedit_ = new QLineEdit();
	apply_user_backname_lineedit_->setGeometry(120, 60, 160, 30);
	apply_user_backname_lineedit_->setStyleSheet("font-size: 14px;");

	// 招呼语
	apply_user_hello_label_ = new QLabel();
	apply_user_hello_label_->setText("招呼语:");
	apply_user_hello_label_->setGeometry(20, 100, 100, 30);
	apply_user_hello_label_->setStyleSheet("font-size: 14px;");

	apply_user_hello_lineedit_ = new QLineEdit();
	apply_user_hello_lineedit_->setGeometry(120, 100, 160, 30);
	apply_user_hello_lineedit_->setStyleSheet("font-size: 14px;");

	content_widget_->addWidget(apply_user_name_label_);
	content_widget_->addWidget(apply_user_name_lineedit_);
	content_widget_->addWidget(apply_user_backname_label_);
	content_widget_->addWidget(apply_user_backname_lineedit_);
	content_widget_->addWidget(apply_user_hello_label_);
	content_widget_->addWidget(apply_user_hello_lineedit_);

	layout->addLayout(content_widget_);

	layout->addSpacerItem(
		new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

	QHBoxLayout* button_layout = new QHBoxLayout();
	apply_button_ = new QPushButton();
	apply_button_->setText("申请");
	apply_button_->setGeometry(20, 120, 80, 30);
	apply_button_->setStyleSheet("font-size: 14px;");
	button_layout->addWidget(apply_button_);

	cancel_button_ = new QPushButton();
	cancel_button_->setText("取消");
	cancel_button_->setGeometry(120, 120, 80, 30);
	cancel_button_->setStyleSheet("font-size: 14px;");
	button_layout->addWidget(cancel_button_);
	layout->addLayout(button_layout);
}