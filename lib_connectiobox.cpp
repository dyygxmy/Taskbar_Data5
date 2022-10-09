#include "lib_connectiobox.h"
#include <QDebug>
#include <QTimer>
#include <QFile>

lib_connectIOBOX::lib_connectIOBOX(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();

    connectStatus = false;//正常连接状态
    connectIOBOX = false;//IO盒连接成功过
    writeStatus = false;//当前状态是要写数据
    ioOUTIN = "ff00";//io默认上输出下输入
    portBoxName = "ttyS2";//IO盒子的串口名
    deviceNum = 99;//设备地址
    ioData = -1;//IO的数据
}

void lib_connectIOBOX::connectIOBOXStart()
{
    for(int i=0;i<8;i++){
        QString upPath = "/root/gpio/UP" + QString::number(i);
        myCreateFile(upPath);
        QString downPath = "/root/gpio/DOWN" + QString::number(i);
        myCreateFile(downPath);

    }
    connectSerialPort();
    QTimer::singleShot(500,this,SLOT(connectManage()));
}


void lib_connectIOBOX::myCreateFile(QString filePath)
{
    QFile file(filePath);
    if (!file.exists()){
            if(file.open( QIODevice::ReadWrite | QIODevice::Text )){
                file.close();
            }
    }
}

void lib_connectIOBOX::connectManage()
{
    int cycle = 50;
    if(connectStatus){
        if(writeStatus){//有需要写就写
            sendFunc(sendType,sendBits,sendLevel);
            writeStatus = false;
        }else{//没有需要写就一直读
            sendFunc(2,0,false);//从0位开始读（读双板）
        }
    }
    QTimer::singleShot(cycle,this,SLOT(connectManage()));
}

void lib_connectIOBOX::connectSerialPort()
{
    QList<QSerialPortInfo>availablePorts = QSerialPortInfo::availablePorts();
//    qDebug()<<"availablePorts:"<<availablePorts.size();
//    foreach (QSerialPortInfo info, availablePorts)
    for(int i = 0;i<availablePorts.size();i++){
        QSerialPortInfo info = availablePorts.at(i);
        QString portName = info.portName();
//        qDebug()<<"serialPort name:"<<portName;//COM3/COM4/COM5
        if(portName == portBoxName){
//            portBoxNameTemp == portName;
            serialPort.setPort(info);
            if(serialPort.isOpen()){
                sendFunc(7,0,true);//查看IO输入/输出状态
//                qDebug()<<"seria port is open:"<<portName;
            }else{
                if(serialPort.open(QIODevice::ReadWrite)){
                    qDebug()<<"setBaudRate:"<<serialPort.setBaudRate(QSerialPort::Baud115200);//每秒位数（B）
                    qDebug()<<"setParity:"<<serialPort.setParity(QSerialPort::NoParity);//奇偶校验（P）
                    qDebug()<<"setDataBits:"<<serialPort.setDataBits(QSerialPort::Data8);//数据位（D）
                    qDebug()<<"setStopBits:"<<serialPort.setStopBits(QSerialPort::OneStop);//停止位（S）
                    qDebug()<<"setFlowControl:"<<serialPort.setFlowControl(QSerialPort::NoFlowControl);//流控制（F）
                    //qDebug()<<"baudRate:"<<serialPort.baudRate();
                    sendFunc(7,0,true);//查看IO输入/输出状态
//                    break;
                    //serialPort.close();
                }else{
                    qDebug()<<"open write serialPort:"<<serialPort.errorString();
                    emit sendConnectErrorIOBox(false,false,"远程IO无法连接");
                    connectStatus = false;
                    QTimer::singleShot(2000,this,SLOT(connectSerialPort()));
                }
            }
            break;
        }else{
            if(i == availablePorts.size() - 1){//遍历串口列表，没有我们指定的串口
                connectStatus = false;
                emit sendConnectErrorIOBox(false,false,"无远程IO接口");
                QTimer::singleShot(2000,this,SLOT(connectSerialPort()));
            }
        }
    }
    if(availablePorts.size() == 0){
        connectStatus = false;
        emit sendConnectErrorIOBox(false,false,"该设备无串口连接");
        QTimer::singleShot(2000,this,SLOT(connectSerialPort()));
    }
}


//类型（type）
//1 读2个板
//2 读上板
//3 读下板
//4 写2个板
//5 写上板
//6 写下板
//7 查看输入输出
void lib_connectIOBOX::sendFunc(int type,int ioNum,bool ONOFF)
{
    writeStruct va;
    va.deviceNum = deviceNum;//设备码目前手动输入，以后用配置文件读入
    if(1 == type){

    }else if(2 == type){//读单板
        va.ctrlNum = 0x01;//读的功能码都是0x01
        va.address_h = 0;
        va.data_h = 0;
        va.address_l = ioNum;//
        va.data_l = 0x10;//0x10 = 16 读16个IO
     /*   if("00ff" == ioOUTIN){//单上输入
            va.address_l = 0;//读地址记录在低地址 0x00读上板或者读双板 0x01读下板
            va.data_l = 0x10;//读的数据记录在低地址 0x01读单板 0x02读双板
        }else if("ff00" == ioOUTIN){//单下输入
            va.address_l = 0;
            va.data_l = 0x10;
        }else if("0000" == ioOUTIN){//上下均作为输入
            va.address_l = 0;
            va.data_l = 0x02;
        }*//*else if(0x00 == ioOUTIN){//上下均作为输出(输出只写，不读)
                va.address_l = 0;
                va.data_l = 0x02;
            }*/
    }else if(3 == type){

    }else if(4 == type){

    }else if(5 == type){

    }else if(6 == type){//写单板
        va.ctrlNum = 0x05;//写的功能码都是0x05
        va.address_h = 0;
        va.address_l = 8 + ioNum;//目前是从8开始
/*        if("ff00" == ioOUTIN){//上输出下输入
//            va.address_l = ioNum + 1;
            va.address_l = 8 + ioNum;
        }else if("00ff" == ioOUTIN){//上输入下输出
            va.address_l = (ioNum + 1) | 0x10;
        }*//*else if(0x00 == ioOUTIN){//上下均作为输出(目前只支持写单板)
            }*//*else if(0x03 == ioOUTIN){//上下均作为输入(输入只读，不写)
            }*/
        if(ONOFF){
            va.data_h = 0xff;//亮灯
        }else{
            va.data_h = 0;//灭灯
        }
        va.data_l = 0;
    }else if(7 == type){//查输入/输出状态
        va.ctrlNum = 0x03;
        va.address_h = 0;
        va.address_l = 0;
        va.data_h = 0;
        va.data_l = 0x01;
    }

    QByteArray buf;
    buf[0] = va.deviceNum;//设备地址
    buf[1] = va.ctrlNum;//功能码
    buf[2] = va.address_h;//高地址
    buf[3] = va.address_l;//低地址
    buf[4] = va.data_h;//寄存器高
    buf[5] = va.data_l;//寄存器低
    int crc = myCheckCRC(buf);
    //    qDebug() << "checkCRC:" << crc;
    buf[6] = crc & 0xff;//CRC低
    buf[7] = crc >> 8;//CRC高
    myWrite(buf,type);
}

int lib_connectIOBOX::myCheckCRC(QByteArray buf)
{
    QString str = buf.toHex();
    int crc = 0XFFFF;
    for(int i = 0;i<buf.size();i++){
        crc ^= str.mid(2*i,2).toInt(0,16);
        for(int j =0;j<8;j++){
            if(1 == (crc & 1)){
                crc = (crc >> 1) ^ 0xA001;
            }else{
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

void lib_connectIOBOX::myWrite(QByteArray buf,int type)
{
//    qDebug()<<"write port:"<<buf.toHex();
    serialPort.write(buf);
    serialPort.waitForReadyRead(1000);
    QString rec = serialPort.readAll().toHex();
//    qDebug()<<"read port:"<<rec;
    if(rec.size() > 0){
        connectIOBOX = true;
        connectStatus = true;
        if(7 == type){//显示输入/输出状态
            ioOUTIN = rec.mid(6,4);
            bool upOut = false;
            bool downOut = false;
            //ff 输出 00 输入
            if("ff" == ioOUTIN.mid(0,2)){//上是否输出
                upOut = true;
            }else if("00" == ioOUTIN.mid(0,2)){
                upOut = false;
            }
            if("ff" == ioOUTIN.mid(2,2)){//下是否输出
                downOut = true;
            }else if("00" == ioOUTIN.mid(2,2)){
                downOut = false;
            }
//            qDebug()<<"readStatus:"<<upOut<<downOut;
            emit sendConnectErrorIOBox(upOut,downOut,"远程IO已连接");
        }else if(2 == type){//显示IO高低电平状态
            int va_readInt = rec.mid(6,4).toInt(0,16);
            if(va_readInt != ioData){
                ioData = va_readInt;//IO数据有变化才发出
                emit sendReceiveReadMsg(rec);//send to lib_IOManage::receiveReadMsg
            }
            fileCheck(va_readInt);
        }
    }else{
        if(connectIOBOX){
            emit sendConnectErrorIOBox(false,false,"远程IO已断开");
        }else{
            emit sendConnectErrorIOBox(false,false,"远程IO设备无连接");
        }
        connectStatus = false;
        QTimer::singleShot(2000,this,SLOT(connectSerialPort()));
    }
}

void lib_connectIOBOX::receiveWriteCMD(int type, int bits, bool level)
{
    sendType = type;//发送串口的控制类型（读，写或其他）
    sendBits = bits;//控制哪一位
    sendLevel = level;//写入时的高低电平
    writeStatus = true;
}

void lib_connectIOBOX::fileCheck(int ioData)
{
    int va_up0 = ioData & 1;
    int va_up1 = ioData>>1 & 1;
    int va_up2 = ioData>>2 & 1;
    int va_up3 = ioData>>3 & 1;
    int va_up4 = ioData>>4 & 1;
    int va_up5 = ioData>>5 & 1;
    int va_up6 = ioData>>6 & 1;
    int va_up7 = ioData>>7 & 1;
//    qDebug()<<"readIO:"<<QString::number(ioData,2);
    QFile UP0File("/root/gpio/UP0");
    if(UP0File.open(QFile::ReadOnly)){
        int va_file = QString(UP0File.readAll()).trimmed().toInt();
        UP0File.close();
        if(va_file != va_up0){
            receiveWriteCMD(6,0,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP0 open error:"<<UP0File.errorString();
    }
    QFile UP1File("/root/gpio/UP1");
    if(UP1File.open(QFile::ReadOnly)){
        int va_file = QString(UP1File.readAll()).trimmed().toInt();
        UP1File.close();
        if(va_file != va_up1){
            receiveWriteCMD(6,1,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP1 open error:"<<UP1File.errorString();
    }
    QFile UP2File("/root/gpio/UP2");
    if(UP2File.open(QFile::ReadOnly)){
        int va_file = QString(UP2File.readAll()).trimmed().toInt();
        UP2File.close();
        if(va_file != va_up2){
            receiveWriteCMD(6,2,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP2 open error:"<<UP2File.errorString();
    }
    QFile UP3File("/root/gpio/UP3");
    if(UP3File.open(QFile::ReadOnly)){
        int va_file = QString(UP3File.readAll()).trimmed().toInt();
        UP3File.close();
        if(va_file != va_up3){
            receiveWriteCMD(6,3,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP3 open error:"<<UP3File.errorString();
    }
    QFile UP4File("/root/gpio/UP4");
    if(UP4File.open(QFile::ReadOnly)){
        int va_file = QString(UP4File.readAll()).trimmed().toInt();
        UP4File.close();
        if(va_file != va_up4){
            receiveWriteCMD(6,4,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP4 open error:"<<UP4File.errorString();
    }
    QFile UP5File("/root/gpio/UP5");
    if(UP5File.open(QFile::ReadOnly)){
        int va_file = QString(UP5File.readAll()).trimmed().toInt();
        UP5File.close();
        if(va_file != va_up5){
            receiveWriteCMD(6,5,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP5 open error:"<<UP5File.errorString();
    }
    QFile UP6File("/root/gpio/UP6");
    if(UP6File.open(QFile::ReadOnly)){
        int va_file = QString(UP6File.readAll()).trimmed().toInt();
        UP6File.close();
        if(va_file != va_up6){
            receiveWriteCMD(6,6,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP6 open error:"<<UP6File.errorString();
    }
    QFile UP7File("/root/gpio/UP7");
    if(UP7File.open(QFile::ReadOnly)){
        int va_file = QString(UP7File.readAll()).trimmed().toInt();
        UP7File.close();
        if(va_file != va_up7){
            receiveWriteCMD(6,7,va_file);
        }
    }else{
        qDebug()<<"/root/gpio/UP7 open error:"<<UP7File.errorString();
    }
}
