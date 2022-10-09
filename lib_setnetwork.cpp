#include "lib_setnetwork.h"
#include "ui_lib_setnetwork.h"
#include <QSettings>
#include <QButtonGroup>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include "unistd.h"
//QString network_ini = "/allMyConfig/network.ini";
//QString DNSPath = "/etc/resolvconf/resolv.conf.d/base";
//QSettings* networkSet = new QSettings(network_ini,QSettings::IniFormat);


lib_setNetwork::lib_setNetwork(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_setNetwork)
{
    ui->setupUi(this);
    ui->but_close->hide();
    openEditFlag = true;//主要控制按钮允许点击

    if(networkSet.contains("baseinfo/connectGateway")){
        GConnectGateway = networkSet.value("baseinfo/connectGateway").toString();
    }else{
        GConnectGateway = "wlan0";
        networkSet.setValue("baseinfo/connectGateway",GConnectGateway);
    }
    if("wlan0" == GConnectGateway){
        ui->raBut_wlan0->setChecked(true);
    }else if("eth0" == GConnectGateway){
        ui->raBut_eth0->setChecked(true);
    }else if("eth1" == GConnectGateway){
        ui->raBut_eth1->setChecked(true);
    }

    //添加ip输入检验
//    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    QRegExp rx("^((([1-9]{0,1}|2[0-4]|1\\d)\\d|25[0-5])(\\.|$)){4}");
    ui->wirelessIP->setValidator(new QRegExpValidator(rx, this));
//    ui->wirelessIP->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->wirelessNetmask->setValidator(new QRegExpValidator(rx, this));
//    ui->wirelessNetmask->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->wirelessGateway->setValidator(new QRegExpValidator(rx, this));
//    ui->wirelessGateway->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
//    ui->DataServerIp->setValidator(new QRegExpValidator(rx, this));
//    ui->DataServerIp->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->DNS1->setValidator(new QRegExpValidator(rx, this));
//    ui->DNS1->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
//    ui->RfidIp->setValidator(new QRegExpValidator(rx, this));
//    ui->RfidIp->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->DNS2->setValidator(new QRegExpValidator(rx, this));
//    ui->DNS2->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->EthernetIP1->setValidator(new QRegExpValidator(rx, this));
//    ui->EthernetIP1->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->EthernetNetmask1->setValidator(new QRegExpValidator(rx, this));
//    ui->EthernetNetmask1->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->EthernetGateway1->setValidator(new QRegExpValidator(rx, this));
//    ui->EthernetGateway1->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->EthernetIP2->setValidator(new QRegExpValidator(rx, this));
//    ui->EthernetIP2->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->EthernetNetmask2->setValidator(new QRegExpValidator(rx, this));
//    ui->EthernetNetmask2->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->EthernetGateway2->setValidator(new QRegExpValidator(rx, this));
//    ui->EthernetGateway2->setInputMask("000.000.000.000;0");//必须加;0否则前面的正则表达式失效，;0”表示删除时默认填充为0
    ui->bridging_ip->setValidator(new QRegExpValidator(rx, this));

    setStyleSheetBatch();

    ui->psk->setEchoMode(QLineEdit::Password);//行编辑器内输入的内容为密码输入形式



    this->setFocusPolicy(Qt::ClickFocus);
    showConfigInfo();

    //有配置就取配置，没配置就配置一个给个默认值 开启NTP功能
//    if(networkSet.contains("baseinfo/CalibrationON")){
//        GOpenNTP = networkSet.value("baseinfo/OpenNTP").toBool();
//    }else{
//        GOpenNTP = false;
//        networkSet.setValue("baseinfo/OpenNTP",GOpenNTP);
//    }
//    if(GOpenNTP){
//        ui->but_openNTP->setStyleSheet("border-image: url(:/new/prefix1/ON.bmp);");
//        ui->but_openNTPDATE->setStyleSheet("border-image: url(:/new/prefix1/OFF.bmp);");
//        emit sendNTPRun();
//    }else{
//        ui->but_openNTP->setStyleSheet("border-image: url(:/new/prefix1/OFF.bmp);");
//        ui->but_openNTPDATE->setStyleSheet("border-image: url(:/new/prefix1/ON.bmp);");
//        emit sendNTPDATERun(ui->ntpServer->text());
//    }

}

lib_setNetwork::~lib_setNetwork()
{
    delete ui;
}

void lib_setNetwork::on_but_close_clicked()
{
#if 0
    QString wirelessSSID = networkSet.value("baseinfo/wirelessSSID").toString();
    QString wirelessPsk = networkSet.value("baseinfo/wirelessPsk").toString();
    QString wirelessIP = networkSet.value("baseinfo/wirelessIP").toString();
    QString wirelessNetmask = networkSet.value("baseinfo/wirelessNetmask").toString();
    QString wirelessGateway = networkSet.value("baseinfo/wirelessGateway").toString();
    QString EthernetIP1 = networkSet.value("baseinfo/EthernetIP1").toString();
    QString EthernetNetmask1 = networkSet.value("baseinfo/EthernetNetmask1").toString();
    QString EthernetGateway1 = networkSet.value("baseinfo/EthernetGateway1").toString();
    QString EthernetIP2 = networkSet.value("baseinfo/EthernetIP2").toString();
    QString EthernetNetmask2 = networkSet.value("baseinfo/EthernetNetmask2").toString();
    QString EthernetGateway2 = networkSet.value("baseinfo/EthernetGateway2").toString();
//    QString DataServerIp = networkSet.value("baseinfo/DataServerIp").toString();
//    QString RfidIp = networkSet.value("baseinfo/RfidIp").toString();
    QString bridgingIP = networkSet.value("baseinfo/bridgingIP").toString();
    QString bridgingNetwork = networkSet.value("baseinfo/bridgingNetwork").toString();
    ui->SSID->setText(wirelessSSID);
    ui->psk->setText(wirelessPsk);
    ui->wirelessIP->setText(wirelessIP);
    ui->wirelessNetmask->setText(wirelessNetmask);
    ui->wirelessGateway->setText(wirelessGateway);
    ui->EthernetIP1->setText(EthernetIP1);
    ui->EthernetNetmask1->setText(EthernetNetmask1);
    ui->EthernetGateway1->setText(EthernetGateway1);
    ui->EthernetIP2->setText(EthernetIP2);
    ui->EthernetNetmask2->setText(EthernetNetmask2);
    ui->EthernetGateway2->setText(EthernetGateway2);
//    ui->DataServerIp->setText(DataServerIp);
//    ui->RfidIp->setText(RfidIp);
    ui->bridging_ip->setText(bridgingIP);
    ui->bridging_network->setText(bridgingNetwork);
    this->hide();
#endif
    showConfigInfo();
}

void lib_setNetwork::on_but_save_clicked()
{
    if(openEditFlag){
        myMsgBox = new myMessageBox;
        myMsgBox->showMsg("保存确认","是否保存所有设置???");
        myMsgBox->show();
        connect(myMsgBox,SIGNAL(sendChoice(bool)),this,SLOT(saveChoice(bool)));
    }
}

void lib_setNetwork::closeEdit()
{
//    qDebug()<<"closeEdit";
    openEditFlag = false;
    ui->raBut_wlan0->setEnabled(false);
    ui->raBut_eth0->setEnabled(false);
    ui->raBut_eth1->setEnabled(false);
    foreach (QObject *cl, ui->groupBox->children()) {
        if(QString(cl->metaObject()->className()) == "QLineEdit"){
            qobject_cast<QLineEdit*>(cl)->setEnabled(false);
        }
    }
}

void lib_setNetwork::openEdit()
{
//    qDebug()<<"openEdit";
    openEditFlag = true;
    ui->raBut_wlan0->setEnabled(true);
    ui->raBut_eth0->setEnabled(true);
    ui->raBut_eth1->setEnabled(true);
    foreach (QObject *cl, ui->groupBox->children()) {
        if(QString(cl->metaObject()->className()) == "QLineEdit"){
            qobject_cast<QLineEdit*>(cl)->setEnabled(true);
        }
    }
}

void lib_setNetwork::saveChoice(bool cho)
{
    if(cho){
        networkSet.setValue("baseinfo/wirelessSSID",ui->SSID->text());
        networkSet.setValue("baseinfo/wirelessPsk",ui->psk->text());
        emit sendChoiceNetwork(ui->SSID->text(),ui->psk->text());
        networkSet.setValue("baseinfo/wirelessIP",ui->wirelessIP->text());
        networkSet.setValue("baseinfo/wirelessNetmask",ui->wirelessNetmask->text());
        networkSet.setValue("baseinfo/wirelessGateway",ui->wirelessGateway->text());
        networkSet.setValue("baseinfo/EthernetIP1",ui->EthernetIP1->text());
        networkSet.setValue("baseinfo/EthernetNetmask1",ui->EthernetNetmask1->text());
        networkSet.setValue("baseinfo/EthernetGateway1",ui->EthernetGateway1->text());
        networkSet.setValue("baseinfo/EthernetIP2",ui->EthernetIP2->text());
        networkSet.setValue("baseinfo/EthernetNetmask2",ui->EthernetNetmask2->text());
        networkSet.setValue("baseinfo/EthernetGateway2",ui->EthernetGateway2->text());
//        networkSet.setValue("baseinfo/DataServerIp",ui->DataServerIp->text());
//        networkSet.setValue("baseinfo/RfidIp",ui->RfidIp->text());
        networkSet.setValue("baseinfo/bridgingIP",ui->bridging_ip->text());
        networkSet.setValue("baseinfo/bridgingNetwork",ui->bridging_network->text());
        //保存DNS
        QString DNS1 = ui->DNS1->text();
        QString DNS2 = ui->DNS2->text();
        QFile dnsWriteFile(DNSPath);
        if(dnsWriteFile.open(QFile::WriteOnly)){
            QTextStream in(&dnsWriteFile);
            if(DNS1.size() >= 7){
                in<<"nameserver "+ DNS1<<endl;
            }
            if(DNS2.size() >= 7){
                in<<"nameserver "+ DNS2<<endl;
            }
        }else{
            qDebug()<<"open dnsWriteFile:"<<dnsWriteFile.errorString();
        }
        setBridging();//桥接设置
    }else{
        showConfigInfo();
    }
    myMsgBox->deleteLater();
//    this->setFocus(Qt::MenuBarFocusReason);//菜单栏成了焦点

}

void lib_setNetwork::showConfigInfo()
{
    QString wirelessSSID = networkSet.value("baseinfo/wirelessSSID").toString();
    QString wirelessPsk = networkSet.value("baseinfo/wirelessPsk").toString();
    QString wirelessIP = networkSet.value("baseinfo/wirelessIP").toString();
    QString wirelessNetmask = networkSet.value("baseinfo/wirelessNetmask").toString();
    QString wirelessGateway = networkSet.value("baseinfo/wirelessGateway").toString();
    QString EthernetIP1 = networkSet.value("baseinfo/EthernetIP1").toString();
    QString EthernetNetmask1 = networkSet.value("baseinfo/EthernetNetmask1").toString();
    QString EthernetGateway1 = networkSet.value("baseinfo/EthernetGateway1").toString();
    QString EthernetIP2 = networkSet.value("baseinfo/EthernetIP2").toString();
    QString EthernetNetmask2 = networkSet.value("baseinfo/EthernetNetmask2").toString();
    QString EthernetGateway2 = networkSet.value("baseinfo/EthernetGateway2").toString();
//    QString ntpServer = networkSet.value("baseinfo/ntpServer").toString();
//    QString DataServerIp = networkSet.value("baseinfo/DataServerIp").toString();
//    QString RfidIp = networkSet.value("baseinfo/RfidIp").toString();
    QString bridgingIP = networkSet.value("baseinfo/bridgingIP").toString();
    QString bridgingNetwork = networkSet.value("baseinfo/bridgingNetwork").toString();
    ui->SSID->setText(wirelessSSID);
    ui->psk->setText(wirelessPsk);
    ui->wirelessIP->setText(wirelessIP);
    ui->wirelessNetmask->setText(wirelessNetmask);
    ui->wirelessGateway->setText(wirelessGateway);
    ui->EthernetIP1->setText(EthernetIP1);
    ui->EthernetNetmask1->setText(EthernetNetmask1);
    ui->EthernetGateway1->setText(EthernetGateway1);
    ui->EthernetIP2->setText(EthernetIP2);
    ui->EthernetNetmask2->setText(EthernetNetmask2);
    ui->EthernetGateway2->setText(EthernetGateway2);
//    ui->DataServerIp->setText(DataServerIp);
//    ui->RfidIp->setText(RfidIp);
    ui->bridging_ip->setText(bridgingIP);
    ui->bridging_network->setText(bridgingNetwork);

    setBridging();

    bridgingIPTemp = bridgingIP;
    bridgingNetworkTemp = bridgingNetwork;
    //获取DNS
    QFile dnsFile(DNSPath);
    if(dnsFile.open(QFile::ReadOnly)){
        QString dnsStr = dnsFile.readAll();
        dnsStr = dnsStr.trimmed();
        if(dnsStr.size() > 0){
            QStringList dnsList = dnsStr.split("\n");
            bool contains_nameserver = false;
            for(int i = 0;i<dnsList.size();i++){
                QString dnsLineStr = dnsList.at(i);
                contains_nameserver = dnsLineStr.contains("nameserver");
                if(contains_nameserver){
                    if(0 == i){
                        ui->DNS1->setText(dnsLineStr.remove("nameserver").trimmed());//DNS1
                    }else if(1 == i){
                        ui->DNS2->setText(dnsLineStr.remove("nameserver").trimmed());//DNS2
                    }else{
                        qDebug()<<"*DNS count > 2*";
                    }
                }
            }
            if(!contains_nameserver){//一个DNS设置都没有
                ui->DNS1->setText("");
                ui->DNS2->setText("");
            }
        }
        dnsFile.close();
    }else{
        qDebug()<<"open dnsFile:"<<dnsFile.errorString();
    }
}


void lib_setNetwork::setStyleSheetBatch()
{
    foreach (QObject *cl, ui->groupBox->children()) {
        if(QString(cl->metaObject()->className()) == "QLineEdit"){
            qobject_cast<QLineEdit*>(cl)->setStyleSheet("font: 14pt \"Arial\";border-width:1px;border-style:solid;border-color:rgb(51, 153, 255);margin-top: 0ex;");
        }
        if(QString(cl->metaObject()->className()) == "QLabel"){
            qobject_cast<QLabel*>(cl)->setStyleSheet("font: 14pt \"黑体\";");
        }
    }
}


void lib_setNetwork::getChoiceNetwork(QString ssid, QString psk)
{
    ui->SSID->setText(ssid);
    ui->psk->setText(psk);
}

void lib_setNetwork::on_pushButton_pressed()
{
//    qDebug()<<"on_pushButton_pressed";
    if(openEditFlag){
        ui->pushButton->setStyleSheet("border-image: url(:/new/prefix1/view.png);");
        ui->psk->setEchoMode(QLineEdit::Normal);
    }
}

void lib_setNetwork::on_pushButton_released()
{
//    qDebug()<<"on_pushButton_released";
    if(openEditFlag){
        ui->pushButton->setStyleSheet("border-image: url(:/new/prefix1/view_off.png);");
        ui->psk->setEchoMode(QLineEdit::Password);
    }
}

void lib_setNetwork::on_raBut_wlan0_clicked()
{
//    qDebug()<<"choice wlan0";
    if(openEditFlag){
        if(GConnectGateway != "wlan0"){
            GConnectGateway = "wlan0";
            networkSet.setValue("baseinfo/connectGateway",GConnectGateway);
        }
    }
}

void lib_setNetwork::on_raBut_eth0_clicked()
{
//    qDebug()<<"choice eth0";
    if(openEditFlag){
        if(GConnectGateway != "eth0"){
            GConnectGateway = "eth0";
            networkSet.setValue("baseinfo/connectGateway",GConnectGateway);
        }
    }
}

void lib_setNetwork::on_raBut_eth1_clicked()
{
//    qDebug()<<"choice eth1";
    if(openEditFlag){
        if(GConnectGateway != "eth1"){
            GConnectGateway = "eth1";
            networkSet.setValue("baseinfo/connectGateway",GConnectGateway);
        }
    }
}

void lib_setNetwork::on_but_cancel_clicked()
{
    if(openEditFlag){
        showConfigInfo();
    }
}

//桥接设置
void lib_setNetwork::setBridging()
{
    QString bridgingIP = ui->bridging_ip->text();
    QString bridgingNetwork = ui->bridging_network->text();
    if(bridgingIP.size() > 0 && bridgingNetwork.size() > 0){
        QString addbrBash = "brctl addbr br0";
        system(addbrBash.toLocal8Bit().data());
        if(bridgingIPTemp != bridgingIP){//IP有变化
            QString setIPBash = "ifconfig br0 " + bridgingIP;
            system(setIPBash.toLocal8Bit().data());
        }
        if(bridgingNetworkTemp != bridgingNetwork){//添加的网卡有变化
            QString delBrBash = "brctl delif br0 wlan0 eth0 eth1";
            system(delBrBash.toLocal8Bit().data());
            QStringList nets = bridgingNetwork.split(",");
            QString addBrNetBash = "brctl addif br0";
            for(int i = 0;i < nets.size();i++){
                addBrNetBash = addBrNetBash + " " + nets.at(i);
            }
            system(addBrNetBash.toLocal8Bit().data());
        }
    }
}
