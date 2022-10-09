#include "connectcp2112.h"
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDateTime>

connectCP2112::connectCP2112(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
}

void connectCP2112::startCP2112()
{
    connectIni();
    QTimer::singleShot(100,this,SLOT(connectManage()));
}

void connectCP2112::connectIni()
{
    EMU = new CP2112_HIDAPI;
    if(EMU->open_device(0x10c4, 0xea90)){
        qDebug()<<"CP2112_HIDAPI open success";
        struct CP2112_HIDAPI::cp2112_configure_struct configuration;
        configuration.clock_divider = 0x0000;
        configuration.gpio0_txtoggle_en = 0;
        configuration.gpio1_rxtoggle_en = 0;
        configuration.gpio7_clk_en = 0;
        configuration.gpio_nopendrain = 0x00;
        configuration.gpio_output_ninput = 0xFF;
        EMU->cp2112_configure(configuration);
        EMU->SMBus_configure();
        memset((void*) &buffer[0], 0x00, sizeof(buffer));
        buffer[0] = 0x01;
        buffer[1] = 0xF0;
        EMU->i2c_write(0xc8, 0x02, buffer);
    }else{
        qDebug()<<"CP2112_HIDAPI open faile";
    }
}

void connectCP2112::connectManage()
{
    int cycle = 50;
    //获取OUT0~OUT7---------------------
    uint8 outStatus = 0;
    EMU->get_gpio(&outStatus);
    outStatus^=0xFF;//取反（默认是1 低电 灯不亮）
    if(USECP2110){
        va_OUT0 = outStatus>>0 & 1;
        va_OUT1 = outStatus>>1 & 1;
        va_OUT2 = outStatus>>2 & 1;
        va_OUT3 = outStatus>>3 & 1;
        va_OUT4 = outStatus>>4 & 1;
        va_OUT5 = outStatus>>5 & 1;
        va_OUT6 = outStatus>>6 & 1;
        va_OUT7 = outStatus>>7 & 1;
    }else{
        va_OUT0 = outStatus>>7 & 1;
        va_OUT1 = outStatus>>6 & 1;
        va_OUT2 = outStatus>>5 & 1;
        va_OUT3 = outStatus>>4 & 1;
        va_OUT4 = outStatus>>3 & 1;
        va_OUT5 = outStatus>>2 & 1;
        va_OUT6 = outStatus>>1 & 1;
        va_OUT7 = outStatus & 1;
    }
    //    qDebug()<<"outStatus:"<<outStatus;
    //获取电压值--------------------
    uint16 voltagebuf[10];
    uint8 bytesReceived = 0;
    uint16 temp = 0;
    uint32 number=0;
    uint8 tmpa = 0;
    uint8 tmpb = 0;
    for(int i=0;i<10;i++ ){
        memset((void*) &buffer[0], 0x00, sizeof(buffer));
        buffer[0] = 0x08;
        EMU->i2c_write_read(0xc8, 1,1, &bytesReceived, buffer);
        tmpa = buffer[0];
        memset((void*) &buffer[0], 0x00, sizeof(buffer));
        buffer[0] = 0x09;
        EMU->i2c_write_read(0xc8, 1,1, &bytesReceived, buffer);
        tmpb = buffer[0];
        temp = (( tmpa << 8) | tmpb);
        voltagebuf[i] = temp;
        temp = 0;
    }
    voltage_sort(voltagebuf,10);
    number = (voltagebuf[2]+voltagebuf[3]+voltagebuf[4]+voltagebuf[5]+voltagebuf[6]+voltagebuf[7])/6;
    double vm = number*23.6/65535;
    if(vm > 11.47){
        va_VOLTAGE = 16;
    }else if(vm > 11.32){
        va_VOLTAGE = 5;
    }else if(vm > 11.12){
        va_VOLTAGE = 4;
    }else if(vm > 10.88){
        va_VOLTAGE = 3;
    }else if(vm >= 10.4){
        va_VOLTAGE = 2;
    }else if(vm < 10.4){
        va_VOLTAGE = 1;
    }
    memset((void*) &voltagebuf[0], 0x00, sizeof(voltagebuf));

    emit sendReadData(outStatus,va_VOLTAGE);

    QFile OUT0File("/root/gpio/OUT0");
    if(OUT0File.open(QFile::ReadOnly)){
        int va_file = QString(OUT0File.readAll()).trimmed().toInt();
        if(va_file != va_OUT0){
            write_gpio(7,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT0";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT0File.close();
    }else{
        qDebug()<<"/root/gpio/OUT0 open error:"<<OUT0File.errorString();
    }
    QFile OUT1File("/root/gpio/OUT1");
    if(OUT1File.open(QFile::ReadOnly)){
        int va_file = QString(OUT1File.readAll()).trimmed().toInt();
        if(va_file != va_OUT1){
            write_gpio(6,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT1";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT1File.close();
    }else{
        qDebug()<<"/root/gpio/OUT1 open error:"<<OUT1File.errorString();
    }
    QFile OUT2File("/root/gpio/OUT2");
    if(OUT2File.open(QFile::ReadOnly)){
        int va_file = QString(OUT2File.readAll()).trimmed().toInt();
        if(va_file != va_OUT2){
            write_gpio(5,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT2";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT2File.close();
    }else{
        qDebug()<<"/root/gpio/OUT2 open error:"<<OUT2File.errorString();
    }
    QFile OUT3File("/root/gpio/OUT3");
    if(OUT3File.open(QFile::ReadOnly)){
        int va_file = QString(OUT3File.readAll()).trimmed().toInt();
        if(va_file != va_OUT3){
            write_gpio(4,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT3";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT3File.close();
    }else{
        qDebug()<<"/root/gpio/OUT3 open error:"<<OUT3File.errorString();
    }
    QFile OUT4File("/root/gpio/OUT4");
    if(OUT4File.open(QFile::ReadOnly)){
        int va_file = QString(OUT4File.readAll()).trimmed().toInt();
        if(va_file != va_OUT4){
            write_gpio(3,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT4";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT4File.close();
    }else{
        qDebug()<<"/root/gpio/OUT4 open error:"<<OUT4File.errorString();
    }
    QFile OUT5File("/root/gpio/OUT5");
    if(OUT5File.open(QFile::ReadOnly)){
        int va_file = QString(OUT5File.readAll()).trimmed().toInt();
        if(va_file != va_OUT5){
            write_gpio(2,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT5";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT5File.close();
    }else{
        qDebug()<<"/root/gpio/OUT5 open error:"<<OUT5File.errorString();
    }
    QFile OUT6File("/root/gpio/OUT6");
    if(OUT6File.open(QFile::ReadOnly)){
        int va_file = QString(OUT6File.readAll()).trimmed().toInt();
        if(va_file != va_OUT6){
            write_gpio(1,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT6";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT6File.close();
    }else{
        qDebug()<<"/root/gpio/OUT6 open error:"<<OUT6File.errorString();
    }
    QFile OUT7File("/root/gpio/OUT7");
    if(OUT7File.open(QFile::ReadOnly)){
        int va_file = QString(OUT7File.readAll()).trimmed().toInt();
        if(va_file != va_OUT7){
            write_gpio(0,va_file);
        }else{
            QString wirte_file = "echo " + QString::number(va_file) +" >/root/gpio/OUT7";
            system(wirte_file.toLocal8Bit().data());
        }
        OUT7File.close();
    }else{
        qDebug()<<"/root/gpio/OUT7 open error:"<<OUT7File.errorString();
    }
    QTimer::singleShot(cycle,this,SLOT(connectManage()));
}


void connectCP2112::write_gpio(int io,int outVa)
{
    if(USECP2110){
        io = 7 - io;
    }
    if(1 == outVa){
        EMU->gpio_low(io);//亮
    }else {
        EMU->gpio_high(io);//灭
    }
}

void connectCP2112::voltage_sort(uint16 a[],int n)
{
    int i , j;
    uint16 temp;
    for(j=0;j<n-1;j++)
    {
        for(i=0;i<n-1-j;i++)
        {
            if(a[i] >= a[i+1])
            {
                temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
            }
        }
    }
}
