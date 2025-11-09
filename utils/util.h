#ifndef UTIL_H
#define UTIL_H

#include <QWidget>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QStyle>
#include <iostream>


// 加载 qss 样式表
static void LoadStyleSheet(QWidget* widget, const QString& file_path){
    // 加载 qss
    QFile qss_file(file_path);
    if (qss_file.open(QIODevice::ReadOnly)) {
        QString qss = QLatin1String(qss_file.readAll());
        widget->setStyleSheet(qss);
        qss_file.close();
    } else {
        qDebug() << "open file:" << qss_file.fileName() << " failed";
        std::cout << "open qss fail failed" << std::endl;
    }
}

// 刷新控件样式
extern std::function<void(QWidget*)> repolish;
// 字符串异或加密/解密函数
extern std::function<QString(QString)> xorString;

#endif