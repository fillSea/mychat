#include "mainwindow.h"
#include <QDebug>
#include "const.h"
#include "register_dialog.h"
#include "net/tcp_mgr.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	// 创建一个CentralWidget, 并将其设置为MainWindow的中心部件
	login_dialog_ = new LoginDialog(this);
	// 创建一个无边框、无标题栏的自定义窗口
	login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	login_dialog_->show();
	// 让登录对话框成为主窗口的中心部件
	setCentralWidget(login_dialog_);

	// 设置登录状态
	cur_ui_state_ = UIStatus::LOGIN;

	// 切换到注册界面
	connect(login_dialog_, &LoginDialog::SigSwitchRegister, this,
			&MainWindow::SlotSwitchRegister);
	//连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::SigSwichChatWindow, this, &MainWindow::SlotSwitchChat);

	// register_dialog_ = new RegisterDialog(this);
	// // 创建一个无边框、无标题栏的自定义窗口
	// register_dialog_->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
	// register_dialog_->show();
	// // 让注册对话框成为主窗口的中心部件
	// setCentralWidget(register_dialog_);
	// cur_ui_state_ = UIStatus::REGISTER;
	// SlotSwitchChat();
	login_dialog_->Login();
}

MainWindow::~MainWindow() {
	// delete ui;
}

void MainWindow::SlotSwitchRegister() {	 // 切换到注册界面
    if (cur_ui_state_ != UIStatus::LOGIN){
        return;
    }

	register_dialog_ = new RegisterDialog(this);
	register_dialog_->hide();
	register_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

	// 让注册对话框成为主窗口的中心部件
	setCentralWidget(register_dialog_);

    // 连接注册界面返回登录界面的信号
    connect(register_dialog_, &RegisterDialog::SigSwitchLogin, this,
            &MainWindow::SlotSwitchLogin);

	// 隐藏登录对话框
	login_dialog_->hide();
	// 显示注册对话框
	register_dialog_->show();

	// 更新当前UI状态
	cur_ui_state_ = UIStatus::REGISTER;
}

// 切换回登录界面
void MainWindow::SlotSwitchLogin() {  // 切换回登录界面
    // 检查当前UI状态是否为注册
    if (cur_ui_state_ != UIStatus::REGISTER) {
        return;
    }
	// 创建一个CentralWidget, 并将其设置为MainWindow的中心部件
	login_dialog_ = new LoginDialog(this);
	// 创建一个无边框、无标题栏的自定义窗口
	login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	setCentralWidget(login_dialog_);
    
    register_dialog_->hide();
	login_dialog_->show();

    // 重新注册切换注册界面信号
    connect(login_dialog_, &LoginDialog::SigSwitchRegister, this,
            &MainWindow::SlotSwitchRegister);


	// 更新当前UI状态
	cur_ui_state_ = UIStatus::LOGIN;
}

// 切换到聊天界面
void MainWindow::SlotSwitchChat(){
	std::cout << "MainWindow::SlotSwitchChat" << std::endl;
    chat_window_ = new ChatWindow(this);
    chat_window_->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(chat_window_);
    login_dialog_->hide();
    chat_window_->show();
    this->setMinimumSize(QSize(1000,700));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    cur_ui_state_ = UIStatus::CHAT;
}