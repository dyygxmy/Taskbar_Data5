#include "lib_taskbar_wi.h"
#include "ui_lib_taskbar_wi.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QFile>
#include <QMessageBox>

lib_taskbar_wi::lib_taskbar_wi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_taskbar_wi)
{
    ui->setupUi(this);
    desktop = QApplication::desktop();
    mySetWindow();
    type_POWER = 0;//记录实时电源状态
    va_VOLTAGE = 0;//记录实时电量
    POWERTypeTemp = -1;//记录本次电源状态
    VOLTAGEValueTemp = -1;//记录本次电量
    clickCount = 0;//点击次数
    closeEditFlag = true;//关闭编辑功能
    showMyWin = 1;//显示指定的窗口（控制窗口该显示的时候显示）
//    isEventHide = false;//事件处理的隐藏
//    objectIndex = -1;//记录第几个按钮
    reOff = new lib_reOFF;
    setNet = new lib_setNetwork;
    ioManage = new lib_IOManage;
    msconf = new lib_msconfig;
    netMng = new lib_networkManage;
    dateTimeMng = new lib_datetimeManage;
    ntp = new ntpManage;
    connect(dateTimeMng,SIGNAL(sendNTPRun()),ntp,SLOT(ntpRun()));
    connect(dateTimeMng,SIGNAL(sendNTPDATERun(QString)),ntp,SLOT(ntpdateRun(QString)));
    connect(ioManage,SIGNAL(sendPOWERToWin(int)),this,SLOT(getPOWERInfo(int)));
    connect(ioManage,SIGNAL(sendVOLTAGEToWin(int)),this,SLOT(getVOLTAGEInfo(int)));
    connect(netMng,SIGNAL(sendScanWlan()),&scan,SLOT(scanStart()));
    connect(&scan,SIGNAL(sendScanResult(QStringList)),netMng,SLOT(getScanList(QStringList)));
    connect(this,SIGNAL(sendCloseEdit()),setNet,SLOT(closeEdit()));
    connect(this,SIGNAL(sendOpenEdit()),setNet,SLOT(openEdit()));
//    connect(this,SIGNAL(sendCloseEdit()),reOff,SLOT(closeEdit()));
//    connect(this,SIGNAL(sendOpenEdit()),reOff,SLOT(openEdit()));
    connect(this,SIGNAL(sendCloseEdit()),msconf,SLOT(closeEdit()));
    connect(this,SIGNAL(sendOpenEdit()),msconf,SLOT(openEdit()));
    connect(netMng,SIGNAL(sendChoiceNetwork(QString,QString)),setNet,SLOT(getChoiceNetwork(QString,QString)));
    connect(setNet,SIGNAL(sendChoiceNetwork(QString,QString)),netMng,SLOT(getChoiceNetwork(QString,QString)));

    connect(&iwconfig,SIGNAL(sendQuality(QString)),this,SLOT(slot_showQuality(QString)));
    QTimer::singleShot(5*1000,&iwconfig,SLOT(iwconfigStart()));

//    qDebug()<<"widgetAdderss:"<<&reOff<<&setNet<<&ioManage<<&msconf;
//    objectNameList<<"lib_reOFF"<<"lib_setNetwork"<<"lib_IOManage"<<"lib_msconfig";
    widgetList<<reOff<<setNet<<ioManage<<msconf<<netMng<<dateTimeMng;
//    qDebug()<<"objectNameList:"<<reOff->objectName()<<setNet.objectName()<<ioManage.objectName()<<msconf.objectName();



//    slot_showPower();
//    slot_showQuality();
    slot_showDateTime();

    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    ui->pushButton_3->setFlat(true);
    ui->pushButton_4->setFlat(true);
    ui->pushButton_5->setFlat(true);
    ui->pushButton_6->setFlat(true);
    ui->pushButton_7->setFlat(true);
    ui->pushButton_8->setFlat(true);
    ui->pushButton_9->setFlat(true);
    ui->pushButton_10->setFlat(true);
    ui->but_power->setFlat(true);
    ui->but_quy->setFlat(true);
    ui->dateTime->setFlat(true);
    ui->but_power->setEnabled(false);//按键不可点击
//    ui->but_quy->setEnabled(false);//按键不可点击
//    ui->dateTime->setEnabled(false);//按键不可点击
//    ui->pushButton_5->setEnabled(false);//按键不可点击
    ui->pushButton_6->setEnabled(false);//按键不可点击
    ui->pushButton_7->setEnabled(false);//按键不可点击
    ui->pushButton_8->setEnabled(false);//按键不可点击
    ui->pushButton_9->setEnabled(false);//按键不可点击
    ui->pushButton_10->setEnabled(false);//按键不可点击

//    ui->pushButton->setStyleSheet("");
//    ui->pushButton_2->setStyleSheet("");
//    ui->pushButton_3->setStyleSheet("");
//    ui->pushButton_4->setStyleSheet("");
//    ui->pushButton_5->setStyleSheet("");
//    ui->pushButton_6->setStyleSheet("");
//    ui->pushButton_7->setStyleSheet("");
//    ui->pushButton_8->setStyleSheet("");
//    ui->pushButton_9->setStyleSheet("");
//    ui->pushButton_10->setStyleSheet("");
//    ui->pushButton->setIcon(QIcon(QPixmap(":/new/prefix1/func-off.png")));
//    ui->pushButton->setIconSize(QSize(20,20));
//    ui->pushButton_2->setIcon(QIcon(QPixmap(":/new/prefix1/func-set.png")));
//    ui->pushButton_2->setIconSize(QSize(20,20));
//    ui->pushButton_3->setIcon(QIcon(QPixmap(":/new/prefix1/func-io.png")));
//    ui->pushButton_3->setIconSize(QSize(20,20));
//    ui->pushButton_4->setIcon(QIcon(QPixmap(":/new/prefix1/func-run.png")));
//    ui->pushButton_4->setIconSize(QSize(20,20));

//    QTimer::singleShot(10,this,SLOT(mySetWindow()));
}

lib_taskbar_wi::~lib_taskbar_wi()
{
    delete ui;
    reOff->deleteLater();
    setNet->deleteLater();
    ioManage->deleteLater();
    msconf->deleteLater();
    netMng->deleteLater();
    ntp->deleteLater();
}

//对窗口界面做部分设置
void lib_taskbar_wi::mySetWindow()
{
    qDebug()<<"desktop:"<<desktop;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setGeometry(0,desktop->height()-48,desktop->width(),48);
    int dateTime_x = this->width()-ui->dateTime->width()-10;
    ui->dateTime->move(dateTime_x,0);
    int quy_x = dateTime_x - ui->but_quy->width() - 10;
    ui->but_quy->move(quy_x,10);
    int power_x = quy_x - ui->but_power->width() - 10;
    ui->but_power->move(power_x,10);
}

//关机/重启
void lib_taskbar_wi::on_pushButton_clicked()
{
    sendWinManage(0);
}

//网络设置
void lib_taskbar_wi::on_pushButton_2_clicked()
{
    sendWinManage(1);
}


//IO管理
void lib_taskbar_wi::on_pushButton_3_clicked()
{
    sendWinManage(2);
}


//启动项管理
void lib_taskbar_wi::on_pushButton_4_clicked()
{
    sendWinManage(3);
}

//开启/关闭编辑
void lib_taskbar_wi::on_pushButton_5_clicked()
{
    clickCount++;
    if(clickCount >= 10){
        clickCount = 0;
        QString msg = "是否打开编辑功能？？？";
        if(closeEditFlag){
            msg = "是否关闭编辑功能？？？";
        }
        myMsgBox = new myMessageBox;
        myMsgBox->showMsg("提醒",msg);
        myMsgBox->show();
        connect(myMsgBox,SIGNAL(sendChoice(bool)),this,SLOT(openChoice(bool)));
    }
}

void lib_taskbar_wi::on_pushButton_6_clicked()
{
    clickCount = 0;
    qDebug()<<"按钮6";
}

void lib_taskbar_wi::on_pushButton_7_clicked()
{
    clickCount = 0;
    qDebug()<<"按钮7";
}

void lib_taskbar_wi::on_pushButton_8_clicked()
{
    clickCount = 0;
    qDebug()<<"按钮8";
}

void lib_taskbar_wi::on_pushButton_9_clicked()
{
    clickCount = 0;
    qDebug()<<"按钮9";
}

void lib_taskbar_wi::on_pushButton_10_clicked()
{
    clickCount = 0;
    qDebug()<<"按钮10";
}


void lib_taskbar_wi::widgetMutex(QWidget *widget)
{
//    qDebug()<<"currentWidget:"<<widget;
    for(int i = 0;i<widgetList.size();i++){
        if(widget == widgetList.at(i)){
            widget->show();
//            qDebug()<<"showWidget:"<<widget;
        }else{
            widgetList.at(i)->hide();
        }
    }
}


//网络信号显示
void lib_taskbar_wi::slot_showQuality(QString quy)
{
//    0    rssi<=-100
//    1    (-100, -88]
//    2    (-88, -77]
//    3    (-66, -55]
//    4    rssi>=-55
    if(quy.size() > 0){
        if(quy.toInt() == 0){//未获取到网络
            ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-00.png);");
        }else if(quy.toInt() >= -55){//4格
            ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-04.png);");
        }else if(quy.toInt() >= -66){//3格
            ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-03.png);");
        }else if(quy.toInt() >= -77){//2格
            ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-02.png);");
        }else if(quy.toInt() >= -88){//1格
            ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-01.png);");
        }else if(quy.toInt() >= -100){//0格
            ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-00.png);");
        }
    }else{//网卡都不存在
        ui->but_quy->setStyleSheet("border-image: url(:/new/prefix1/wlan-error.png);");
    }
}

//电池电量显示
void lib_taskbar_wi::slot_showPower()
{
    if(POWERTypeTemp != type_POWER || VOLTAGEValueTemp != va_VOLTAGE){
        if(1 == type_POWER){//接电源
            if(16 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-35.png);");
            }else if(5 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-34.png);");
            }else if(4 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-33.png);");
            }else if(3 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-32.png);");
            }else if(2 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-31.png);");
            }
        }else if(0 == type_POWER){//未接电源，用电池
            if(16 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-05.png);");
            }else if(5 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-04.png);");
            }else if(4 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-03.png);");
            }else if(3 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-02.png);");
            }else if(2 == va_VOLTAGE){
                ui->but_power->setStyleSheet("border-image: url(:/new/prefix1/power-01.png);");
            }
        }

        if(VOLTAGEValueTemp == 2 && va_VOLTAGE == 1){//电量低到1关机
            reOff->poweriffFunc();
        }

        POWERTypeTemp = type_POWER;
        VOLTAGEValueTemp = va_VOLTAGE;
    }
}

//时间显示
void lib_taskbar_wi::slot_showDateTime()
{
    int cycle = 100;
    ui->dateTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss\nyyyy-MM-dd"));
    QTimer::singleShot(cycle,this,SLOT(slot_showDateTime()));
}

void lib_taskbar_wi::getPOWERInfo(int POWER)
{
    type_POWER = POWER;
    slot_showPower();
}

void lib_taskbar_wi::getVOLTAGEInfo(int VOLTAGE)
{
    va_VOLTAGE = VOLTAGE;
}

//打开时间设置
void lib_taskbar_wi::on_dateTime_clicked()
{
    sendWinManage(5);
}

void lib_taskbar_wi::sendWinManage(int objectIndex)
{
    clickCount = 0;
//    qDebug()<<"sendWinManage:"<<objectIndex<<showMyWin;
    if(showMyWin){
        if(widgetList.at(objectIndex)->isHidden()){
            widgetList.at(objectIndex)->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            int width = 100;
            int height = 100;
            int x = 0;
            int y = 0;
            if(0 == objectIndex){
                x = 0;
                y = 0;
                width = desktop->width();
                height = desktop->height();
                widgetList.at(objectIndex)->setGeometry(x,y,width,height);
                reOff->setIni();
            }else if(4 == objectIndex){
                x = desktop->width() - widgetList.at(objectIndex)->width();
                y = desktop->height() - widgetList.at(objectIndex)->height() - this->height();
                width = widgetList.at(objectIndex)->width();
                height = widgetList.at(objectIndex)->height();
                widgetList.at(objectIndex)->setGeometry(x,y,width,height);
                netMng->scanWlanStart();
            }else if(5 == objectIndex){
                x = desktop->width() - widgetList.at(objectIndex)->width();
                y = desktop->height() - widgetList.at(objectIndex)->height() - this->height();
                width = widgetList.at(objectIndex)->width();
                height = widgetList.at(objectIndex)->height();
                widgetList.at(objectIndex)->setGeometry(x,y,width,height);
            }else{
                width = 700;
                height = 500;
                x = 0;
                y = desktop->height() - height - this->height();
                widgetList.at(objectIndex)->setGeometry(x,y,width,height);
            }
            widgetList.at(objectIndex)->setFocus(Qt::MenuBarFocusReason);//菜单栏成了焦点(防止窗口打开焦点直接跳到第一个Edit控件)
            widgetMutex(widgetList.at(objectIndex));
        }else{
            widgetList.at(objectIndex)->hide();
        }
    }
    showMyWin = 1;
}

//是否打开/关闭编辑功能
void lib_taskbar_wi::openChoice(bool cho)
{
//    qDebug()<<"openChoice[]";
    if(cho){
        if(closeEditFlag){
            closeEdit();
            closeEditFlag = false;
        }else{
            openEdit();
            closeEditFlag = true;//关闭编辑功能
        }
    }
    myMsgBox->deleteLater();
}

void lib_taskbar_wi::openEdit()
{
//    qDebug()<<"sendOpenEdit";
    emit sendOpenEdit();
}

void lib_taskbar_wi::closeEdit()
{
//    qDebug()<<"sendCloseEdit";
    emit sendCloseEdit();
}

//打开网络界面
void lib_taskbar_wi::on_but_quy_clicked()
{
    sendWinManage(4);
}

//不让点击事件触发
void lib_taskbar_wi::slot_noClicked()
{
//    qDebug()<<"slot_noClicked";
    showMyWin = 0;
}
