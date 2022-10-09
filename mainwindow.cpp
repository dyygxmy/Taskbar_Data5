#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "../../DWKeyboard/include/KeyboardGlobal.h"
#include "lib_taskbar_wi.h"
#include <QDesktopWidget>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QDesktopWidget *desktop = QApplication::desktop();
//    qDebug()<<desktop->width()<<desktop->height();

//    lib_taskbar_wi wi;
//    wi.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//    wi.setGeometry(0,desktop->height()-50,1366,50);

//    GlobalInit();
//    wi;
//    ui->lineEdit->installEventFilter(this);


//    wi.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//bool MainWindow::eventFilter(QObject *watched, QEvent *event)
//{
//    if (event->type()==QEvent::FocusIn)
//    {
//        PlatformInputContextBase->FocusIn(watched);
//    }
//    else if (event->type()==QEvent::FocusOut)
//    {
//        PlatformInputContextBase->FocusOut(watched);
//    }

//    return QWidget::eventFilter(watched,event);
//}
