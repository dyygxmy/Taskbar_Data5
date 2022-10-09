#include "connectcp2110.h"
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDateTime>

connectcp2110::connectcp2110(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
    readALLCP2110();
}


void connectcp2110::startCP2110()
{

}


void connectcp2110::readALLCP2110()
{
    int ret = 0;
    QFile IN1File("/root/gpio/IN1");
    if(IN1File.open(QFile::ReadOnly)){
        int va_file = QString(IN1File.readAll()).trimmed().toInt();
        IN1File.close();
        ret += (va_file^1)<<7;
    }
    QFile IN2File("/root/gpio/IN2");
    if(IN2File.open(QFile::ReadOnly)){
        int va_file = QString(IN2File.readAll()).trimmed().toInt();
        IN2File.close();
        ret += (va_file^1)<<6;
    }
    QFile IN3File("/root/gpio/IN3");
    if(IN3File.open(QFile::ReadOnly)){
        int va_file = QString(IN3File.readAll()).trimmed().toInt();
        IN3File.close();
        ret += (va_file^1)<<5;
    }
    QFile IN4File("/root/gpio/IN4");
    if(IN4File.open(QFile::ReadOnly)){
        int va_file = QString(IN4File.readAll()).trimmed().toInt();
        IN4File.close();
        ret += (va_file^1)<<4;
    }
    QFile IN0File("/root/gpio/IN0");
    if(IN0File.open(QFile::ReadOnly)){
        int va_file = QString(IN0File.readAll()).trimmed().toInt();
        IN0File.close();
        ret += (va_file^1)<<3;
    }
    QFile COM0File("/root/gpio/COM0");
    if(COM0File.open(QFile::ReadOnly)){
        int va_file = QString(COM0File.readAll()).trimmed().toInt();
        COM0File.close();
        ret += va_file<<2;
    }
    QFile COM1File("/root/gpio/COM1");
    if(COM1File.open(QFile::ReadOnly)){
        int va_file = QString(COM1File.readAll()).trimmed().toInt();
        COM1File.close();
        ret += va_file<<1;
    }
    QFile POWERFile("/root/gpio/POWER");
    if(POWERFile.open(QFile::ReadOnly)){
        int va_file = QString(POWERFile.readAll()).trimmed().toInt();
        POWERFile.close();
        ret += (va_file^1)<<0;
    }
    emit sendCP2110Msg(ret);
    QTimer::singleShot(50,this,SLOT(readALLCP2110()));
}

void connectcp2110::writeCP2110(int gpio,int value)
{

}
