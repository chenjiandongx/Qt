#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,510);
    this->setWindowTitle("无标题 - 记事本");
    // 设置无边框编辑区域
    setCentralWidget(ui->textEdit);
    // 隐藏状态栏
    ui->statusBar->hide();
    // 文件选项槽
    QObject::connect(ui->newAction,SIGNAL(triggered()),this,SLOT(newFileSlot()));
    QObject::connect(ui->openAction,SIGNAL(triggered()),this,SLOT(openFileSlot()));
    QObject::connect(ui->saveAction,SIGNAL(triggered()),this,SLOT(saveFileSlot()));
    QObject::connect(ui->saveAsAction,SIGNAL(triggered()),this,SLOT(saveFileAsSlot()));
    QObject::connect(ui->exitAction,SIGNAL(triggered()),this,SLOT(close()));

    // 编辑选项槽
    QObject::connect(ui->dateAction,SIGNAL(triggered()),this,SLOT(currentDateTimeSlot()));
    QObject::connect(ui->undoAction,SIGNAL(triggered()),ui->textEdit,SLOT(undo()));
    QObject::connect(ui->redoAction,SIGNAL(triggered()),ui->textEdit,SLOT(redo()));
    QObject::connect(ui->undoAction,SIGNAL(triggered()),ui->textEdit,SLOT(undo()));
    QObject::connect(ui->cutActioin,SIGNAL(triggered()),ui->textEdit,SLOT(cut()));
    QObject::connect(ui->pasteAction,SIGNAL(triggered()),ui->textEdit,SLOT(paste()));
    QObject::connect(ui->copyAction,SIGNAL(triggered()),ui->textEdit,SLOT(copy()));
    QObject::connect(ui->selectAllAction,SIGNAL(triggered()),ui->textEdit,SLOT(selctAll()));

    // 格式选项槽
    QObject::connect(ui->colorAction,SIGNAL(triggered()),this,SLOT(setColorSlot()));
    QObject::connect(ui->fontAction,SIGNAL(triggered()),this,SLOT(setFontSlot()));

    // 帮助选项槽
    // qApp为QCoreApplication::instance();即应用程序实例化全局指针
    QObject::connect(ui->aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    QObject::connect(ui->aboutAction,SIGNAL(triggered()),this,SLOT(aboutSlot()));
    QObject::connect(ui->webAction,SIGNAL(triggered()),this,SLOT(webSlot()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

// 新建文件
void MainWindow::newFileSlot()
{
    // 判断当前文档内容是否被改变
    if(ui->textEdit->document()->isModified())
    {
        // 清空编辑区域
        ui->textEdit->clear();
        // 设置文本
        this->setWindowTitle("无标题.txt");
    }
}

//打开文件
void MainWindow::openFileSlot()
{
    // 获取当前文件名称
    openFileName = QFileDialog::getOpenFileName(this,"Open File");
    QFileInfo fileInfo = QFileInfo(openFileName);
    QString fileName = fileInfo.fileName();

    if(openFileName.isEmpty())
    {
        QMessageBox::information(this,"提示","Please select a text file");
        return;
    }
    QFile *file = new QFile;
    file->setFileName(openFileName);
    // 设置为只读权限
    bool ok = file->open(QIODevice::ReadOnly);
    if(ok)
    {
        this->setWindowTitle(fileName+" - 记事本");
        // 将QTextStream与文件进行关联
        QTextStream in(file);
        // 读取所有文本内容
        ui->textEdit->setText(in.readAll());
        // 关闭文件并删除实例
        file->close();
        delete file;
    }
    else
    {
        QMessageBox::information(this,"提示","File open error!"+file->errorString());
    }

}

//另存为文件
void MainWindow::saveFileAsSlot()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,"Save File",QDir::currentPath(),tr("文本文档(*.txt);;所有文件(*)"));
    // 判断文件是否存在
    if(saveFileName.isEmpty())
    {
        QMessageBox::information(this,"提示","Please select a file!      ");
        return;
    }
    QFile *file = new QFile;
    file->setFileName(saveFileName);
    bool ok = file->open(QIODevice::WriteOnly);
    if(ok)
    {
       // 将QTextStream与文件进行关联
       QTextStream out(file);
       // 将文件内容转换为纯文本
       out << ui->textEdit->toPlainText();
       // 关闭并删除文件实例
       file->close();
       delete file;
    }
    else
    {
        QMessageBox::information(this,"Error Message","File save error!"+file->errorString());
        return;
    }
}

// 保存文件
void MainWindow::saveFileSlot()
{
    // 判断要保存的文件是否已经存在，如果不存在，则执行另存为操作
    if(saveFileName.isEmpty())
    {
        this->saveFileAsSlot();
    }
    else
    {
        QFile *file = new QFile;
        file->setFileName(saveFileName);
        // 设置为只写权限
        bool ok = file->open(QIODevice::WriteOnly);
        if(ok)
        {
           // 将QTextStream与文件进行关联
           QTextStream out(file);
           // 将文件内容转换为纯文本
           out << ui->textEdit->toPlainText();
           // 关闭文件并删除实例
           file->close();
           delete file;
        }
    }
}

// 打印文件
void MainWindow::printFileSlot()
{

}

// 设置字体
void MainWindow::setFontSlot()
{
    bool ok;
    // 获得字体
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 12), this);
    if (ok)
    {
        // 设置字体
        ui->textEdit->setFont(font);
    }
    else {
        // QMessageBox::information(this,"Error Message","Error set font      ");
        return;
    }
}

// 设置颜色
void MainWindow::setColorSlot()
{
    // 获得颜色
     QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");
     if (color.isValid())
     {
        // 设置颜色
        ui->textEdit->setTextColor(color);
     }
     else
     {
       // QMessageBox::information(this,"Error Message","Color is unvalid!       ");
       return;
     }
}

// 设置日期
void MainWindow::currentDateTimeSlot()
{
    // 获得当前时间
    QDateTime current = QDateTime::currentDateTime();
    // 转换时间格式
    QString time = current.toString("yyyy-MM-dd hh:mm:ss");
    // 将时间追加到文本下一行
    ui->textEdit->append(time);
}

// 关于
void MainWindow::aboutSlot()
{
    about *dialog = new about;
    dialog->setWindowTitle("关于");
    // 非模态对话框（互不干涉）
    dialog->show();
    // 模态对话框
    // dialog->exec();
}

// 官网
void MainWindow::webSlot()
{
    QDesktopServices::openUrl(QUrl("http://weibo.com/qqace?is_all=1"));
}

// 退出事件
void MainWindow::closeEvent(QCloseEvent* event)
{
    // 判断文本的内容是否已经被修改
    if(ui->textEdit->document()->isModified()) {
    QMessageBox msgBox;
    // 如果未执行保存操作，则文件名为空，显示为无标题
    if(openFileName == "")
    {
        openFileName = "无标题";
    }
    // 设置消息对话框字体
    QFont font("微软雅黑", 11);
    msgBox.setFont(font);

    msgBox.setWindowTitle("记事本");
    msgBox.setInformativeText("是否将更改保存到 "+openFileName);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setButtonText(2048,"保存");
    msgBox.setButtonText(8388608,"不保存");
    msgBox.setButtonText(4194304,"取消");
    // 默认的焦点为save
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret)
    {
      case QMessageBox::Save:
          // 执行保存操作
          this->saveFileSlot();
          break;
      case QMessageBox::Discard:
          // 不保存则丢弃文件，事件接受，程序关闭
          event->accept();
          break;
      case QMessageBox::Cancel:
          // 取消则忽略事件
          event->ignore();
          break;
      default:
          break;
        }
    }
    else
    {
        event->accept();
    }
}
