#include "connectyanhuaio.h"
#include <QTimer>
#include <QFile>
#include <QDebug>

connectYanhuaIO::connectYanhuaIO(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
    pvalue = 0;
    pvalueTemp = 255;
    isWriteHandle = false;//向IO写数据
}

void connectYanhuaIO::connectStart()
{
    SusiLibInitialize();//启用研华IO控制
    SusiGPIOSetDirection(0x10000, 0xff, 0b11111001);//设置输入/输出模式为1111 1001
    QTimer::singleShot(100,this,SLOT(connecttManage()));
}

void connectYanhuaIO::connecttManage()
{
    int cycle = 50;
    if(isWriteHandle){
        isWriteHandle = false;
        writeIO(iPin, type, value);
        if(iPin == 2){
            if(value == 0){
                system("echo 0 >/root/gpio/COM0");
            }else{
                system("echo 1 >/root/gpio/COM0");
            }
        }else if(iPin == 1){
            if(value == 0){
                system("echo 0 >/root/gpio/COM1");
            }else{
                system("echo 1 >/root/gpio/COM1");
            }
        }
        cycle = 10;
    }else{
        writeIO(0,1,0);
        if(pvalueTemp != pvalue){//值有变化才发到界面处理（显示并存文件）
            pvalueTemp = pvalue;
            emit sendYanhuaIOMsg(pvalue);
        }
        int va_COM0 = pvalue>>2 & 1;//输出信号1高电平，0低电平
        int va_COM1 = pvalue>>1 & 1;//输出信号1高电平，0低电平
        //判断2个输出口的文件是否有变化
        QFile COM1File("/root/gpio/COM1");
        if(COM1File.open(QFile::ReadOnly)){
            int va_file = QString(COM1File.readAll()).trimmed().toInt();
            COM1File.close();
            if(va_file != va_COM1){
                writeIO(1,0,va_file);
            }
        }else{
            qDebug()<<"/root/gpio/COM1 open error:"<<COM1File.errorString();
        }
        QFile COM0File("/root/gpio/COM0");
        if(COM0File.open(QFile::ReadOnly)){
            int va_file = QString(COM0File.readAll()).trimmed().toInt();
            COM0File.close();
//            qDebug()<<"read com0 file:"<<va_file<<QString::number(pvalue,2);
            if(va_file != va_COM0){
                writeIO(2,0,va_file);
            }
        }else{
            qDebug()<<"/root/gpio/COM0 open error:"<<COM0File.errorString();
        }
    }
    QTimer::singleShot(cycle,this,SLOT(connecttManage()));
}

//iPin 操作哪些IO type输入1/输出0 value操作的值 mode多个1/单个0
void connectYanhuaIO::getSetIO(int iPin, int type, int value,int mode)
{
    this->iPin = iPin;
    this->type = type;
    this->value = value;
    isWriteHandle = true;
}

void connectYanhuaIO::writeIO(int iPin, int type, int value)
{
//    qDebug()<<"writeIO:"<<iPin<<type<<value;
    int mask = 0xff;
    if(1 == type){//输入 读IO，目前是全读
        iPin = 65536;
        SusiGPIOGetLevel(iPin, mask, &pvalue);
    }else if(0 == type){//输出 写IO，目前只操作单写
        SusiGPIOSetLevel(iPin, mask, value);
    }
//    qDebug()<<"pvalue:"<<QString::number(pvalue,2);
}
