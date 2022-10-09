#include "lib_iomanage.h"
#include "ui_lib_iomanage.h"
#include "unistd.h"
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QList>
#include <QDir>

lib_IOManage::lib_IOManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_IOManage)
{
    ui->setupUi(this);
    ui->lineEdit->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->but_closeiobox->hide();
    ui->but_connectiobox->hide();
    ui->but_sendiobox->hide();
    ui->iobox_cycle->hide();
    ui->iobox_readData->hide();
    ui->iobox_writeData->hide();
    ui->comboBox->hide();
    ui->raBut_a->hide();
    ui->raBut_m->hide();
    ui->gpio_allValue->hide();
    printVM = true;//打印一次电量

    QString gpioPath = "/root/gpio/";
    QDir dir(gpioPath);
    if(!dir.exists()){
        dir.mkpath(gpioPath);
    }

    ioBox = new lib_connectIOBOX;
    connect(ioBox,SIGNAL(sendConnectErrorIOBox(bool,bool,QString)),this,SLOT(connectErrorIOBox(bool,bool,QString)));
    connect(ioBox,SIGNAL(sendReceiveReadMsg(QString)),this,SLOT(receiveReadMsg(QString)));
    connect(this,SIGNAL(sendFunc(int,int,bool)),ioBox,SLOT(receiveWriteCMD(int,int,bool)));
    QTimer::singleShot(10,ioBox,SLOT(connectIOBOXStart()));

    cp2112 = new connectCP2112;
    connect(cp2112,SIGNAL(sendReadData(int,int)),this,SLOT(receiveCP2112ReadMsg(int,int)));
    connect(this,SIGNAL(sendWriteCP2112(int,int)),cp2112,SLOT(write_gpio(int,int)));
    QTimer::singleShot(10,cp2112,SLOT(startCP2112()));

    yanhuaIO = new connectYanhuaIO;
    connect(yanhuaIO,SIGNAL(sendYanhuaIOMsg(int)),this,SLOT(receiveYanhuaIOReadMsg(int)));
    connect(this,SIGNAL(sendWriteYanhuaIO(int,int,int,int)),yanhuaIO,SLOT(getSetIO(int,int,int,int)));
#if OPENYANHUAIO
    if(USECP2110^1){
        QTimer::singleShot(10,yanhuaIO,SLOT(connectStart()));
    }
#endif

    cp2110 = new connectcp2110;
    connect(cp2110,SIGNAL(sendCP2110Msg(int)),this,SLOT(receiveYanhuaIOReadMsg(int)));

//    msg_readRecIOBOX = "";//读IO盒子返回的报文
//    va_readIOBOX = "";//读IO盒子的数据
//    setIONum = 7;//输出IO号
}

lib_IOManage::~lib_IOManage()
{
    delete ui;
    ioBox->deleteLater();
    cp2112->deleteLater();
    yanhuaIO->deleteLater();
}

void lib_IOManage::on_but_OUT0_clicked()
{
    if(1 == va_OUT0){
        system("echo 0 >/root/gpio/OUT0");
        emit sendWriteCP2112(7,0);
    }else{
        system("echo 1 >/root/gpio/OUT0");
        emit sendWriteCP2112(7,1);
    }
}

void lib_IOManage::on_but_OUT1_clicked()
{
    if(1 == va_OUT1){
        system("echo 0 >/root/gpio/OUT1");
        emit sendWriteCP2112(6,0);
    }else{
        system("echo 1 >/root/gpio/OUT1");
        emit sendWriteCP2112(6,1);
    }
}

void lib_IOManage::on_but_OUT2_clicked()
{
    if(1 == va_OUT2){
        system("echo 0 >/root/gpio/OUT2");
        emit sendWriteCP2112(5,0);
    }else{
        system("echo 1 >/root/gpio/OUT2");
        emit sendWriteCP2112(5,1);
    }
}

void lib_IOManage::on_but_OUT3_clicked()
{
    if(1 == va_OUT3){
        system("echo 0 >/root/gpio/OUT3");
        emit sendWriteCP2112(4,0);
    }else{
        system("echo 1 >/root/gpio/OUT3");
        emit sendWriteCP2112(4,1);
    }
}

void lib_IOManage::on_but_OUT4_clicked()
{
    if(1 == va_OUT4){
        system("echo 0 >/root/gpio/OUT4");
        emit sendWriteCP2112(3,0);
    }else{
        system("echo 1 >/root/gpio/OUT4");
        emit sendWriteCP2112(3,1);
    }
}

void lib_IOManage::on_but_OUT5_clicked()
{
    if(1 == va_OUT5){
        system("echo 0 >/root/gpio/OUT5");
        emit sendWriteCP2112(2,0);
    }else{
        system("echo 1 >/root/gpio/OUT5");
        emit sendWriteCP2112(2,1);
    }
}

void lib_IOManage::on_but_OUT6_clicked()
{
    if(1 == va_OUT6){
        system("echo 0 >/root/gpio/OUT6");
        emit sendWriteCP2112(1,0);
    }else{
        system("echo 1 >/root/gpio/OUT6");
        emit sendWriteCP2112(1,1);
    }
}

void lib_IOManage::on_but_OUT7_clicked()
{
    if(1 == va_OUT7){
        system("echo 0 >/root/gpio/OUT7");
        emit sendWriteCP2112(0,0);
    }else{
        system("echo 1 >/root/gpio/OUT7");
        emit sendWriteCP2112(0,1);
    }
}

void lib_IOManage::on_but_COM0_clicked()
{
    if(USECP2110){
        if(1 == va_COM0){
            system("echo 0 >/root/gpio/COM0");
        }else{
            system("echo 1 >/root/gpio/COM0");
        }
    }else{
        if(1 == va_COM0){
            emit sendWriteYanhuaIO(2,0,0,0);
        }else{
            emit sendWriteYanhuaIO(2,0,1,0);
        }
    }
}

void lib_IOManage::on_but_COM1_clicked()
{
    if(USECP2110){
        if(1 == va_COM1){
            system("echo 0 >/root/gpio/COM1");
        }else{
            system("echo 1 >/root/gpio/COM1");
        }
    }else{
        if(1 == va_COM1){
            emit sendWriteYanhuaIO(1,0,0,0);
        }else{
            emit sendWriteYanhuaIO(1,0,1,0);
        }
    }
}

void lib_IOManage::on_but_UP0_clicked()
{
    if(1 == va_up0){
        system("echo 0 >/root/gpio/UP0");
        emit sendFunc(6,0,false);
    }else{
        system("echo 1 >/root/gpio/UP0");
        emit sendFunc(6,0,true);
    }
}

void lib_IOManage::on_but_UP1_clicked()
{
    if(1 == va_up1){
        system("echo 0 >/root/gpio/UP1");
        emit sendFunc(6,1,false);
    }else{
        system("echo 1 >/root/gpio/UP1");
        emit sendFunc(6,1,true);
    }
}

void lib_IOManage::on_but_UP2_clicked()
{
    if(1 == va_up2){
        system("echo 0 >/root/gpio/UP2");
        emit sendFunc(6,2,false);
    }else{
        system("echo 1 >/root/gpio/UP2");
        emit sendFunc(6,2,true);
    }
}

void lib_IOManage::on_but_UP3_clicked()
{
    if(1 == va_up3){
        system("echo 0 >/root/gpio/UP3");
        emit sendFunc(6,3,false);
    }else{
        system("echo 1 >/root/gpio/UP3");
        emit sendFunc(6,3,true);
    }
}

void lib_IOManage::on_but_UP4_clicked()
{
    if(1 == va_up4){
        system("echo 0 >/root/gpio/UP4");
        emit sendFunc(6,4,false);
    }else{
        system("echo 1 >/root/gpio/UP4");
        emit sendFunc(6,4,true);
    }
}

void lib_IOManage::on_but_UP5_clicked()
{
    if(1 == va_up5){
        system("echo 0 >/root/gpio/UP5");
        emit sendFunc(6,5,false);
    }else{
        system("echo 1 >/root/gpio/UP5");
        emit sendFunc(6,5,true);
    }
}

void lib_IOManage::on_but_UP6_clicked()
{
    if(1 == va_up6){
        system("echo 0 >/root/gpio/UP6");
        emit sendFunc(6,6,false);
    }else{
        system("echo 1 >/root/gpio/UP6");
        emit sendFunc(6,6,true);
    }
}

void lib_IOManage::on_but_UP7_clicked()
{
    if(1 == va_up7){
        system("echo 0 >/root/gpio/UP7");
        emit sendFunc(6,7,false);
    }else{
        system("echo 1 >/root/gpio/UP7");
        emit sendFunc(6,7,true);
    }
}

void lib_IOManage::on_but_DOWN0_clicked()
{
    if(1 == va_down0){
        emit sendFunc(6,0,false);
    }else{
        emit sendFunc(6,0,true);
    }
}

void lib_IOManage::on_but_DOWN1_clicked()
{
    if(1 == va_down1){
        emit sendFunc(6,1,false);
    }else{
        emit sendFunc(6,1,true);
    }
}

void lib_IOManage::on_but_DOWN2_clicked()
{
    if(1 == va_down2){
        emit sendFunc(6,2,false);
    }else{
        emit sendFunc(6,2,true);
    }
}

void lib_IOManage::on_but_DOWN3_clicked()
{
    if(1 == va_down3){
        emit sendFunc(6,3,false);
    }else{
        emit sendFunc(6,3,true);
    }
}

void lib_IOManage::on_but_DOWN4_clicked()
{
    if(1 == va_down4){
        emit sendFunc(6,4,false);
    }else{
        emit sendFunc(6,4,true);
    }
}

void lib_IOManage::on_but_DOWN5_clicked()
{
    if(1 == va_down5){
        emit sendFunc(6,5,false);
    }else{
        emit sendFunc(6,5,true);
    }
}

void lib_IOManage::on_but_DOWN6_clicked()
{
    if(1 == va_down6){
        emit sendFunc(6,6,false);
    }else{
        emit sendFunc(6,6,true);
    }
}

void lib_IOManage::on_but_DOWN7_clicked()
{
    if(1 == va_down7){
        emit sendFunc(6,7,false);
    }else{
        emit sendFunc(6,7,true);
    }
}

void lib_IOManage::on_but_readIO_clicked()
{
    emit sendFunc(2,0,false);//从0位开始读（读双板）
}

void lib_IOManage::on_but_readIO_2_clicked()
{
    emit sendFunc(2,8,false);//从8位开始读，读下板
}

void lib_IOManage::on_but_readIO_3_clicked()
{
//    emit sendFunc(2,0,false);
}
//IO盒子连接信息的处理
void lib_IOManage::connectErrorIOBox(bool upOut , bool downOut , QString errorData)
{
    qDebug()<<"connectErrorIOBox:"<<upOut<<downOut<<errorData;
    ui->groupBox_3->setTitle(errorData);
    if(upOut){
        ui->but_UP0->setText("OUT0");
        ui->but_UP1->setText("OUT1");
        ui->but_UP2->setText("OUT2");
        ui->but_UP3->setText("OUT3");
        ui->but_UP4->setText("OUT4");
        ui->but_UP5->setText("OUT5");
        ui->but_UP6->setText("OUT6");
        ui->but_UP7->setText("OUT7");
        ui->but_UP0->setEnabled(true);
        ui->but_UP1->setEnabled(true);
        ui->but_UP2->setEnabled(true);
        ui->but_UP3->setEnabled(true);
        ui->but_UP4->setEnabled(true);
        ui->but_UP5->setEnabled(true);
        ui->but_UP6->setEnabled(true);
        ui->but_UP7->setEnabled(true);
    }else{
        ui->but_UP0->setText("IN0");
        ui->but_UP1->setText("IN1");
        ui->but_UP2->setText("IN2");
        ui->but_UP3->setText("IN3");
        ui->but_UP4->setText("IN4");
        ui->but_UP5->setText("IN5");
        ui->but_UP6->setText("IN6");
        ui->but_UP7->setText("IN7");
        ui->but_UP0->setEnabled(false);
        ui->but_UP1->setEnabled(false);
        ui->but_UP2->setEnabled(false);
        ui->but_UP3->setEnabled(false);
        ui->but_UP4->setEnabled(false);
        ui->but_UP5->setEnabled(false);
        ui->but_UP6->setEnabled(false);
        ui->but_UP7->setEnabled(false);
    }
    if(downOut){
        ui->but_DOWN0->setText("OUT0");
        ui->but_DOWN1->setText("OUT1");
        ui->but_DOWN2->setText("OUT2");
        ui->but_DOWN3->setText("OUT3");
        ui->but_DOWN4->setText("OUT4");
        ui->but_DOWN5->setText("OUT5");
        ui->but_DOWN6->setText("OUT6");
        ui->but_DOWN7->setText("OUT7");
        ui->but_DOWN0->setEnabled(true);
        ui->but_DOWN1->setEnabled(true);
        ui->but_DOWN2->setEnabled(true);
        ui->but_DOWN3->setEnabled(true);
        ui->but_DOWN4->setEnabled(true);
        ui->but_DOWN5->setEnabled(true);
        ui->but_DOWN6->setEnabled(true);
        ui->but_DOWN7->setEnabled(true);
    }else{
        ui->but_DOWN0->setText("IN0");
        ui->but_DOWN1->setText("IN1");
        ui->but_DOWN2->setText("IN2");
        ui->but_DOWN3->setText("IN3");
        ui->but_DOWN4->setText("IN4");
        ui->but_DOWN5->setText("IN5");
        ui->but_DOWN6->setText("IN6");
        ui->but_DOWN7->setText("IN7");
        ui->but_DOWN0->setEnabled(false);
        ui->but_DOWN1->setEnabled(false);
        ui->but_DOWN2->setEnabled(false);
        ui->but_DOWN3->setEnabled(false);
        ui->but_DOWN4->setEnabled(false);
        ui->but_DOWN5->setEnabled(false);
        ui->but_DOWN6->setEnabled(false);
        ui->but_DOWN7->setEnabled(false);
    }
}

void lib_IOManage::receiveReadMsg(QString msg)
{
//    msg_readRecIOBOX = msg;
//    va_readIOBOX = msg.mid(6,4);
    //获取IO盒子数据
    int va_readInt = msg.mid(6,4).toInt(0,16);
//    qDebug()<<"va_readBit:"<<QString::number(va_redaIOBOX.toInt(0,16),2);
    va_up0 = va_readInt & 1;
    va_up1 = va_readInt>>1 & 1;
    va_up2 = va_readInt>>2 & 1;
    va_up3 = va_readInt>>3 & 1;
    va_up4 = va_readInt>>4 & 1;
    va_up5 = va_readInt>>5 & 1;
    va_up6 = va_readInt>>6 & 1;
    va_up7 = va_readInt>>7 & 1;
    va_down0 = va_readInt>>8 & 1;
    va_down1 = va_readInt>>9 & 1;
    va_down2 = va_readInt>>10 & 1;
    va_down3 = va_readInt>>11 & 1;
    va_down4 = va_readInt>>12 & 1;
    va_down5 = va_readInt>>13 & 1;
    va_down6 = va_readInt>>14 & 1;
    va_down7 = va_readInt>>15 & 1;
    if(msg.size() > 0){//IO连接着 输出信号的文件不在这里写
        if(1 == va_up0){
            ui->but_UP0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP0");
        }else{
            ui->but_UP0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP0");
        }
        if(1 == va_up1){
            ui->but_UP1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP1");
        }else{
            ui->but_UP1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP1");
        }
        if(1 == va_up2){
            ui->but_UP2->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP2");
        }else{
            ui->but_UP2->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP2");
        }
        if(1 == va_up3){
            ui->but_UP3->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP3");
        }else{
            ui->but_UP3->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP3");
        }
        if(1 == va_up4){
            ui->but_UP4->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP4");
        }else{
            ui->but_UP4->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP4");
        }
        if(1 == va_up5){
            ui->but_UP5->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP5");
        }else{
            ui->but_UP5->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP5");
        }
        if(1 == va_up6){
            ui->but_UP6->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP6");
        }else{
            ui->but_UP6->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP6");
        }
        if(1 == va_up7){
            ui->but_UP7->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
//            system("echo 1 >/root/gpio/UP7");
        }else{
            ui->but_UP7->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
//            system("echo 0 >/root/gpio/UP7");
        }
        if(1 == va_down0){
            ui->but_DOWN0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN0");
        }else{
            ui->but_DOWN0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN0");
        }
        if(1 == va_down1){
            ui->but_DOWN1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN1");
        }else{
            ui->but_DOWN1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN1");
        }
        if(1 == va_down2){
            ui->but_DOWN2->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN2");
        }else{
            ui->but_DOWN2->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN2");
        }
        if(1 == va_down3){
            ui->but_DOWN3->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN3");
        }else{
            ui->but_DOWN3->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN3");
        }
        if(1 == va_down4){
            ui->but_DOWN4->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN4");
        }else{
            ui->but_DOWN4->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN4");
        }
        if(1 == va_down5){
            ui->but_DOWN5->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN5");
        }else{
            ui->but_DOWN5->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN5");
        }
        if(1 == va_down6){
            ui->but_DOWN6->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN6");
        }else{
            ui->but_DOWN6->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN6");
        }
        if(1 == va_down7){
            ui->but_DOWN7->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/DOWN7");
        }else{
            ui->but_DOWN7->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/DOWN7");
        }
    }
//    qDebug()<<"readRecMsg:"<<msg;
    //            ui->lineEdit_IODATA->setText(QString::number(va_redaIOBOX.toInt(0,16),2));

}

void lib_IOManage::receiveCP2112ReadMsg(int outStatus, int vm)
{
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
    va_VOLTAGE = vm;
    emit sendVOLTAGEToWin(va_VOLTAGE);
    if(1 == va_OUT0){
        ui->but_OUT0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT1){
        ui->but_OUT1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT2){
        ui->but_OUT2->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT2->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT3){
        ui->but_OUT3->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT3->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT4){
        ui->but_OUT4->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT4->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT5){
        ui->but_OUT5->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT5->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT6){
        ui->but_OUT6->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT6->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    if(1 == va_OUT7){
        ui->but_OUT7->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
    }else{
        ui->but_OUT7->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
    }
    ui->but_VOLTAGE->setText("VOL:"+QString::number(va_VOLTAGE));
    QString write_VOLTAGE = "echo " + QString::number(va_VOLTAGE) + " >/root/gpio/VOLTAGE";
    system(write_VOLTAGE.toLocal8Bit().data());
}

void lib_IOManage::receiveYanhuaIOReadMsg(int pvalue)
{
#if DYTEST3^1
//    ui->gpio_allValue->setText(QString::number(pvalue,2));
#endif
    va_COM0 = pvalue>>2 & 1;//输出信号1高电平，0低电平
    va_COM1 = pvalue>>1 & 1;//输出信号1高电平，0低电平
    pvalue^=0xFF;//输入信号0高电平，1低电平
    va_IN1 = pvalue>>7 & 1;
    va_IN2 = pvalue>>6 & 1;
    va_IN3 = pvalue>>5 & 1;
    va_IN4 = pvalue>>4 & 1;
    va_IN0 = pvalue>>3 & 1;
    va_POWER = pvalue & 1;
    emit sendPOWERToWin(va_POWER);
    if(USECP2110^1){
        if(1 == va_IN1){
            ui->but_IN1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/IN1");
        }else{
            ui->but_IN1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/IN1");
        }
        if(1 == va_IN2){
            ui->but_IN2->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/IN2");
        }else{
            ui->but_IN2->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/IN2");
        }
        if(1 == va_IN3){
            ui->but_IN3->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/IN3");
        }else{
            ui->but_IN3->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/IN3");
        }
        if(1 == va_IN4){
            ui->but_IN4->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/IN4");
        }else{
            ui->but_IN4->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/IN4");
        }
        if(1 == va_IN0){
            ui->but_IN0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/IN0");
        }else{
            ui->but_IN0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/IN0");
        }
        if(1 == va_COM1){
            system("echo 1 >/root/gpio/COM1");
            ui->but_COM1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            system("echo 0 >/root/gpio/COM1");
            ui->but_COM1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_COM0){
            system("echo 1 >/root/gpio/COM0");
            ui->but_COM0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            system("echo 0 >/root/gpio/COM0");
            ui->but_COM0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_POWER){
            ui->but_POWER->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
            system("echo 1 >/root/gpio/POWER");
        }else{
            ui->but_POWER->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
            system("echo 0 >/root/gpio/POWER");
        }
    }else{
        if(1 == va_IN1){
            ui->but_IN1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_IN1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_IN2){
            ui->but_IN2->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_IN2->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_IN3){
            ui->but_IN3->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_IN3->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_IN4){
            ui->but_IN4->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_IN4->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_IN0){
            ui->but_IN0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_IN0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_COM1){
            ui->but_COM1->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_COM1->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_COM0){
            ui->but_COM0->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_COM0->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
        if(1 == va_POWER){
            ui->but_POWER->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/bk-color_7.bmp);font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        }else{
            ui->but_POWER->setStyleSheet("font: 14pt \"Arial\";border-image: url(:/prefix2/LCD_CS351/35_all/69.bmp);color: rgb(255, 255, 255);");
        }
    }
}

//测试手动发送到远程IO
void lib_IOManage::on_but_sendiobox_clicked()
{

}

//连接远程IO
void lib_IOManage::on_but_connectiobox_clicked()
{

}

//断开远程IO
void lib_IOManage::on_but_closeiobox_clicked()
{

}

//选择手动
void lib_IOManage::on_raBut_m_clicked()
{

}

//选择自动
void lib_IOManage::on_raBut_a_clicked()
{

}
