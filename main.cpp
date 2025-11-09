#include <QApplication>

#include "mainwindow.h"
#include <windows.h>
#include <QtCore>


int main(int argc, char *argv[]) {
	// 创建控制台窗口（仅在Windows平台）
    #ifdef Q_OS_WIN
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    #endif

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
