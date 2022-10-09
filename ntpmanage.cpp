#include "ntpmanage.h"
#include <QTimer>
#include "unistd.h"
#include <QDebug>

ntpManage::ntpManage(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
}

void ntpManage::ntpRun()
{
    system("service ntp start");
}

void ntpManage::ntpdateRun(QString ip)
{
    system("service ntp stop");
    system(QString("ntpdate "+ ip).toLocal8Bit().data());
    system("hwclock -w");
    QTimer::singleShot(1*24*60*60*1000,this,SLOT(ntpdateRun(ip)));
}
