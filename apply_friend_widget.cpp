#include "apply_friend_widget.h"
#include <qboxlayout.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include "apply_friend_item.h"
#include "user_mgr.h"
#include "auth_friend_dialog.h"
#include "net/tcp_mgr.h"

ApplyFriendWidget::ApplyFriendWidget(QWidget *parent): QWidget(parent){
    SetUI();
    AddApplyList();
    //接受tcp传递的authrsp信号处理
    connect(TcpMgr::GetInstance().get(), &TcpMgr::SigAuthRsp, this, &ApplyFriendWidget::SlotAuthRsp);
}

void ApplyFriendWidget::AddApplyList(){
    //添加好友申请
    auto apply_list = UserMgr::GetInstance()->GetApplyList();
    std::cout << "apply_list size: " << apply_list.size() << std::endl;
    for(auto &apply: apply_list){
        auto* apply_item = new ApplyFriendItem();
        apply_item->SetApplyInfo(apply);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        apply_friend_list_->insertItem(0,item);
        apply_friend_list_->setItemWidget(item, apply_item);
        if(apply->status_){
            apply_item->ShowAddBtn(false);
        }else{
             apply_item->ShowAddBtn(true);
             auto uid = apply_item->GetUid();
             unauth_items_[uid] = apply_item;
        }

        //收到审核好友信号
        connect(apply_item, &ApplyFriendItem::SigAuthFriend, [this](std::shared_ptr<ApplyInfo> apply_info) {
            auto* authFriend = new AuthFriendDialog(this);
            authFriend->setModal(true);
            authFriend->SetApplyInfo(apply_info);
            authFriend->show();
        });
    }
    /*
    // 模拟假数据，创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *apply_item = new ApplyFriendItem();
        auto apply = std::make_shared<ApplyInfo>("", names[name_i], strs[str_i],
                                    heads[head_i], names[name_i], 0, 1);
        apply_item->SetApplyInfo(apply);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        apply_friend_list_->addItem(item);
        apply_friend_list_->setItemWidget(item, apply_item);
        // 收到审核好友信号
        connect(apply_item, &ApplyFriendItem::SigAuthFriend, [this](std::shared_ptr<ApplyInfo> apply_info){
           auto *authFriend =  new AuthFriendDialog(this);
        //    authFriend->setModal(true);
           authFriend->SetApplyInfo(apply_info);
           authFriend->show();
        });
    }
        */
}

void ApplyFriendWidget::SlotAuthRsp(std::shared_ptr<AuthRsp> auth_rsp) {
    auto uid = auth_rsp->uid_;
    auto find_iter = unauth_items_.find(uid);
    if (find_iter == unauth_items_.end()) {
        return;
    }

    find_iter->second->ShowAddBtn(false);
}

void ApplyFriendWidget::SetUI(){
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* hbox_layout = new QHBoxLayout();
    apply_friend_label_ = new QLabel(this);
    apply_friend_label_->setText("新的朋友");
    hbox_layout->addWidget(apply_friend_label_);
    // 添加一个横向弹簧，将标签推到左侧
    hbox_layout->addStretch();
    layout->addLayout(hbox_layout);

    QVBoxLayout* vbox_layout = new QVBoxLayout();
    apply_friend_list_ = new ApplyFriendList(this);
    vbox_layout->addWidget(apply_friend_list_);
    layout->addLayout(vbox_layout);
}
