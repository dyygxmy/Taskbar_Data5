#include "mycombobox.h"
#include <QDebug>

myComboBox::myComboBox(QWidget *parent):QComboBox(parent)
{

}


void myComboBox::showPopup()
{
    this->clear();
    //this->addItems(QStringList()<<"1");
    QString bash = "ping -w 3 8.8.8.8";//test1
    //QString bash = "ls /";//test2
    //QString bash = "iwlist wlan0 scan";
    process.start(bash);
    if(process.waitForFinished(6000)){
        QString rec = process.readAllStandardOutput();
//        qDebug()<<"readAllStandardOutput:";
        this->addItems(rec.split("\n"));
        QComboBox::showPopup();
    }
}
