#include "lib_reoff.h"
#include "ui_lib_reoff.h"
#include <QPainter>
#include <QDebug>

lib_reOFF::lib_reOFF(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_reOFF)
{
    ui->setupUi(this);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
//    ui->but_poweroff->setFlat(true);
//    ui->but_reboot->setFlat(true);
}

lib_reOFF::~lib_reOFF()
{
    delete ui;
}

//重启
void lib_reOFF::on_but_reboot_clicked()
{
//    qDebug()<<"on_but_reboot_clicked";
    process.execute("reboot");
//    qDebug()<<"on_but_reboot_clicked end";
}

//关机
void lib_reOFF::on_but_poweroff_clicked()
{
    process.execute("poweroff");
}


void lib_reOFF::rebootFunc()
{
   process.execute("reboot");
}

void lib_reOFF::poweriffFunc()
{
    process.execute("poweroff");
}

void lib_reOFF::closeEdit()
{
    ui->but_poweroff->setEnabled(false);
    ui->but_reboot->setEnabled(false);
}

void lib_reOFF::openEdit()
{
    ui->but_poweroff->setEnabled(true);
    ui->but_reboot->setEnabled(true);
}

void lib_reOFF::paintEvent(QPaintEvent * enent)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 200));  //QColor最后一个参数80代表背景的透明度
}

void lib_reOFF::setIni()
{
    int width = this->width()/4;
    int height = this->height()/4;
    ui->but_poweroff->setGeometry(width/2,(this->height() - height)/2,width,height);
    ui->but_reboot->setGeometry(this->width()/2 + width/2,(this->height() - height)/2,width,height);
}
