#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chat_window.h"
#include "const.h"
#include "login_dialog.h"
#include "register_dialog.h"


// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
private slots:
	void SlotSwitchRegister();	// 切换到注册界面
	// 切换回登录界面
	void SlotSwitchLogin();
	// 切换到聊天界面
	void SlotSwitchChat();

private:
	UIStatus cur_ui_state_;	 // 当前UI状态
private:
	// Ui::MainWindow *ui;
	LoginDialog* login_dialog_;		   // 登录界面
	RegisterDialog* register_dialog_;  // 注册界面
	// 聊天界面
	ChatWindow* chat_window_;
};
#endif	// MAINWINDOW_H
