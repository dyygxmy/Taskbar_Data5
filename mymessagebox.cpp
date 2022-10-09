#include "mymessagebox.h"
#include "ui_mymessagebox.h"
#include <QPainter>
#include <QDebug>
//#include <QDesktopWidget>

myMessageBox::myMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myMessageBox)
{
    ui->setupUi(this);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    desktop = QApplication::desktop();
//    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
//    QDesktopWidget *desktop = QApplication::desktop();
    this->setGeometry(0,0,desktop->width(),desktop->height());
    ui->widget->move((desktop->width()-ui->widget->width())/2,(desktop->height()-ui->widget->height())/2);

    ui->widget_2->hide();
}

myMessageBox::~myMessageBox()
{
    delete ui;
}

void myMessageBox::paintEvent(QPaintEvent * enent)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 150));  //QColor最后一个参数代表背景的透明度
}

void myMessageBox::on_but_yes_clicked()
{
    emit sendChoice(true);
}

void myMessageBox::on_but_no_clicked()
{
    emit sendChoice(false);
}

void myMessageBox::on_but_cancel_clicked()
{
    emit sendChoice(false);
}

void myMessageBox::showMsg(QString title,QString msg)
{
    ui->label_title->setText("  " + title);
    ui->label_msg->setText(msg);
}
