#ifndef LIB_CONNECTIOBOX_H
#define LIB_CONNECTIOBOX_H

#include <QObject>
#include <QThread>
#include "lib_taskbar_global.h"
#include <QSerialPort>
#include <QSerialPortInfo>
typedef struct{
    int deviceNum;
    int ctrlNum;
    int address_h;
    int address_l;
    int data_h;
    int data_l;
    int crc;
    int spare1;
    int spare2;
    int spare3;
}writeStruct;
class lib_connectIOBOX : public QObject
{
    Q_OBJECT
public:
    explicit lib_connectIOBOX(QObject *parent = 0);

signals:
    void sendConnectErrorIOBox(bool,bool,QString);
    void sendReceiveReadMsg(QString);
public slots:
    void connectIOBOXStart();
    void receiveWriteCMD(int,int,bool);
private slots:
    void connectManage();
    void connectSerialPort();
private:
    void myCreateFile(QString);
    bool connectStatus;//正常连接状态
    bool writeStatus;//当前状态是要写数据
    bool connectIOBOX;//IO盒连接成功过
    QString portBoxName;//IO盒子的串口名
    QSerialPort serialPort;//连接IO盒子的串口
    QSerialPort::SerialPortError serialPortError;
    QString ioOUTIN;//io输入输出
    int deviceNum;//设备地址
    QThread thread;
    void sendFunc(int,int,bool);
    int myCheckCRC(QByteArray);
    void myWrite(QByteArray,int);
    int ioData;//IO的数据
    int sendType;//发送串口的控制类型（读，写或其他）
    int sendBits;//控制哪一位
    bool sendLevel;//写入时的高低电平
    void fileCheck(int);//文件检查
};

#endif // LIB_CONNECTIOBOX_H
