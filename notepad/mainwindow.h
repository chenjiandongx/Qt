#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>
#include <QDateTime>
#include <QUrl>
#include <QDesktopServices>     // 提供桌面所有运行程序服务
#include <QCloseEvent>
#include <QFileInfo>
#include <QSize>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
// 事件都为保护类型
protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::MainWindow *ui;
    QString saveFileName;
    QString openFileName;

private slots:
    void newFileSlot();             // 新建
    void saveFileSlot();            // 保存
    void saveFileAsSlot();          // 另存为
    void printFileSlot();           // 打印
    void openFileSlot();            // 打开
    void setFontSlot();             // 字体
    void setColorSlot();            // 颜色
    void currentDateTimeSlot();     // 时间和日期
    void aboutSlot();               // 关于
    void webSlot();                 // 官网

};

#endif // MAINWINDOW_H
