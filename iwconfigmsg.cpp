#include "iwconfigmsg.h"
#include <QTimer>

iwconfigMsg::iwconfigMsg(QObject *parent) : QObject(parent)
{
    process = new QProcess(this);
    this->moveToThread(&thread);
    thread.start();
}

void iwconfigMsg::iwconfigStart()
{
    int cycle = 1*1000;
    iwconfigRun();
    QTimer::singleShot(cycle,this,SLOT(iwconfigStart()));
}

void iwconfigMsg::iwconfigRun()
{
    QString quy = "";
    QString bash = "iwconfig";
    QString key = "Signal level=";
    process->start(bash);
    if(process->waitForFinished(1000)){
        QString re = process->readAllStandardOutput();
        if(re.contains("ESSID:")){
            QStringList reList = re.split("\n");
            for(int i = 0;i<reList.size();i++){
                QString line = reList.at(i);
                if(line.contains(key)){
                    quy = line.mid(line.indexOf(key)+key.size(),3);
                    break;
                }
            }
        }
    }
    emit sendQuality(quy);
}
