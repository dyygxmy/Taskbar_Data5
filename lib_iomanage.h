#ifndef LIB_IOMANAGE_H
#define LIB_IOMANAGE_H
#include <QWidget>
#include <QProcess>
#include "lib_taskbar_global.h"
#include "lib_connectiobox.h"
#include "connectcp2112.h"
#include "connectyanhuaio.h"
#include "connectcp2110.h"

namespace Ui {
class lib_IOManage;
}

class lib_IOManage : public QWidget
{
    Q_OBJECT

public:
    explicit lib_IOManage(QWidget *parent = 0);
    ~lib_IOManage();

private slots:
    void on_but_OUT0_clicked();

    void on_but_OUT1_clicked();

    void on_but_OUT2_clicked();

    void on_but_OUT3_clicked();

    void on_but_OUT4_clicked();

    void on_but_OUT5_clicked();

    void on_but_OUT6_clicked();

    void on_but_OUT7_clicked();

    void on_but_COM0_clicked();

    void on_but_COM1_clicked();

    void on_but_UP0_clicked();

    void on_but_UP1_clicked();

    void on_but_UP2_clicked();

    void on_but_UP3_clicked();

    void on_but_UP4_clicked();

    void on_but_UP5_clicked();

    void on_but_UP6_clicked();

    void on_but_UP7_clicked();

    void on_but_DOWN0_clicked();

    void on_but_DOWN1_clicked();

    void on_but_DOWN2_clicked();

    void on_but_DOWN3_clicked();

    void on_but_DOWN4_clicked();

    void on_but_DOWN5_clicked();

    void on_but_DOWN6_clicked();

    void on_but_DOWN7_clicked();

    void on_but_readIO_clicked();

    void on_but_readIO_2_clicked();

    void on_but_readIO_3_clicked();


    void on_but_sendiobox_clicked();

    void on_but_connectiobox_clicked();

    void on_but_closeiobox_clicked();

    void on_raBut_m_clicked();

    void on_raBut_a_clicked();

private:
    Ui::lib_IOManage *ui;

//    QString portBoxNameTemp;//记录本次串口名

//    int setIONum;//输出IO号
//    void connectSerialPort();

//    QProcess process;
    //yanhua
    int va_POWER;
    int va_COM1;
    int va_COM0;
    int va_IN0;
    int va_IN4;
    int va_IN3;
    int va_IN2;
    int va_IN1;
    connectYanhuaIO *yanhuaIO;
    //cp2112
    int va_OUT0;
    int va_OUT1;
    int va_OUT2;
    int va_OUT3;
    int va_OUT4;
    int va_OUT5;
    int va_OUT6;
    int va_OUT7;
    int va_VOLTAGE;
    connectCP2112 * cp2112;



    connectcp2110 * cp2110;

//    typedef struct {
//        uint32_t idType;
//        uint32_t directionVal;
//        uint32_t levelVal;
//    } GPIOConfig, *PGPIOConfig;

//    GPIOConfig config;


    //IO BOX
    int va_up0;
    int va_up1;
    int va_up2;
    int va_up3;
    int va_up4;
    int va_up5;
    int va_up6;
    int va_up7;
    int va_down0;
    int va_down1;
    int va_down2;
    int va_down3;
    int va_down4;
    int va_down5;
    int va_down6;
    int va_down7;
//    QString va_readIOBOX;//读IO盒子的数据
//    QString msg_readRecIOBOX;//读IO盒子返回的报文
    lib_connectIOBOX *ioBox;
    bool printVM;//打印一次电量
signals:
    void sendPOWERToWin(int);
    void sendVOLTAGEToWin(int);
    void sendFunc(int,int,bool);
    void sendWriteCP2112(int,int);
    void sendWriteYanhuaIO(int,int,int,int);
public slots:
    void connectErrorIOBox(bool,bool,QString);//IO盒子未连接的处理
    void receiveReadMsg(QString);
    void receiveCP2112ReadMsg(int,int);
    void receiveYanhuaIOReadMsg(int);
};

#endif // LIB_IOMANAGE_H
