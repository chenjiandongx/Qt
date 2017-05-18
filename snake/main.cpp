#include "snake.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("logo.ico");
    QSplashScreen spalsh(pixmap);
    // 启动此显示图片
    spalsh.show();
    // 使程序在显示启动画面的同时仍能响应鼠标等其他事件
    a.processEvents();
    Snake w;
    w.show();
    // 结束启动画面
    spalsh.finish(&w);
    return a.exec();
}
