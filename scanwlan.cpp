#include "scanwlan.h"
#include <QDebug>


scanWlan::scanWlan(QObject *parent) : QObject(parent)
{
    process = new QProcess(this);

    this->moveToThread(&thread);
    thread.start();
}

scanWlan::~scanWlan()
{
    process->deleteLater();
}

void scanWlan::scanStart()
{
//    qDebug()<<"scanStart:"<<process<<&thread;
    QString bash = "iwlist wlan0 scan";
    process->start(bash);
    QStringList list;
    if(process->waitForFinished(15000)){
        QString rec = process->readAllStandardOutput();

//        qDebug()<<"readAllStandardOutput:"<<rec.size();
        QStringList allList = rec.split("\n");
        for(int i = 0;i<allList.size();i++){
            QString lineData = allList.at(i);
            if(lineData.contains("ESSID:")){
                QString va_SSID = lineData.right(lineData.size()-lineData.indexOf("ESSID:")-7).remove("\"");
                if(va_SSID.size() > 0){
                    list<<va_SSID;
                }
            }
        }
    }
#if DYTEST1
    list<<"WIFI0"<<"WIFI1"<<"WIFI2"<<"WIFI3"<<"WIFI4"<<"WIFI5"<<"WIFI10"<<"WIFI11"<<"WIFI12"<<"WIFI13"<<"WIFI14"<<"WIFI15"<<"WIFI20"<<"WIFI21"<<"WIFI22"<<"WIFI23"<<"WIFI24"<<"WIFI25"<<"WIFI30"<<"WIFI31"<<"WIFI32"<<"WIFI33"<<"WIFI34"<<"WIFI35";
#endif
    emit sendScanResult(list);
}
