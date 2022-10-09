#ifndef CONNECTCP2112_H
#define CONNECTCP2112_H

#include <QObject>
#include <QThread>
#include "gpio_manage/cp2112_hidapi.h"
#include <lib_taskbar_global.h>
class connectCP2112 : public QObject
{
    Q_OBJECT
public:
    explicit connectCP2112(QObject *parent = 0);

signals:
    void sendReadData(int,int);
public slots:
    void startCP2112();
    void write_gpio(int,int);
private slots:
    void connectManage();
private:
    QThread thread;
    CP2112_HIDAPI *EMU;
    void connectIni();
    int va_OUT0;
    int va_OUT1;
    int va_OUT2;
    int va_OUT3;
    int va_OUT4;
    int va_OUT5;
    int va_OUT6;
    int va_OUT7;
    int va_VOLTAGE;
    uint8  buffer[10];
    void voltage_sort(uint16 a[],int n);
};

#endif // CONNECTCP2112_H
