#include "lib_networkmanage.h"
#include "ui_lib_networkmanage.h"
#include <QDebug>
lib_networkManage::lib_networkManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_networkManage)
{
    ui->setupUi(this);
    ui->listWidget->setFrameShape(QListWidget::NoFrame);//隐藏边框
    scrollBar = new QScrollBar;
    scrollBar->setStyleSheet("width:30px;");
    ui->listWidget->setVerticalScrollBar(scrollBar);//设置垂直滚动条
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->SSID->setFocusPolicy(Qt::NoFocus);//设置它不可获得焦点。OK.不可编辑,又能查看完整的显示文本
    ui->psk->setFocusPolicy(Qt::NoFocus);//设置它不可获得焦点。OK.不可编辑,又能查看完整的显示文本
    ui->psk->setEchoMode(QLineEdit::Password);//行编辑器内输入的内容为密码形式
    ui->hotspot_psk->setEchoMode(QLineEdit::Password);
    ui->SSID->setText(networkSet.value("baseinfo/wirelessSSID").toString());
    ui->psk->setText(networkSet.value("baseinfo/wirelessPsk").toString());

    networkMode = networkSet.value("baseinfo/networkMode").toInt();
    if(networkMode){
        ui->stackedWidget->setCurrentIndex(1);
        ui->raBut_hotspot->setChecked(true);
        setHotspotIni();
    }else{
        ui->stackedWidget->setCurrentIndex(0);
        ui->raBut_wifi->setChecked(true);
    }
}

lib_networkManage::~lib_networkManage()
{
    delete ui;
    scrollBar->deleteLater();
}

void lib_networkManage::scanWlanStart()
{
    ui->label->setText("搜索网络中...");
    emit sendScanWlan();
}

void lib_networkManage::getScanList(QStringList list)
{
//    qDebug()<<"getScanList:"<<list;
    if(list.size() > 0){
        ui->label->setText("搜索网络列表如下：");
        wirelessSSID = networkSet.value("baseinfo/wirelessSSID").toString();
        wirelessPsk = networkSet.value("baseinfo/wirelessPsk").toString();
        if(list.contains(wirelessSSID)){
            list.removeAt(list.indexOf(wirelessSSID));
            list.replace(0,wirelessSSID);
        }
    }else{
        ui->label->setText("未搜索到网络，请重试");
    }

    ui->listWidget->clear();
    for(int i = 0;i < list.size();i++){
        ui->listWidget->addItem(list.at(i));
        ui->listWidget->item(i)->setSizeHint(QSize(300,50));
    }
}

void lib_networkManage::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(previous){
        previous->setText(choiceSSID);
        previous->setSizeHint(QSize(300,50));
        ui->listWidget->removeItemWidget(previous);
    }
//    qDebug()<<"on_listWidget_currentItemChanged"<<current<<previous;
    if(current){
        choiceSSID = current->text();
        current->setText("");
        current->setSizeHint(QSize(300,150));
        widget = new QWidget;
        widget->setGeometry(0,0,300,150);
//        widget->setStyleSheet("background-color: rgb(255, 255, 255,100);color: rgb(255, 255, 255);font: 14pt \"Arial\";");
        label_itemText = new QLabel(widget);
        label_itemText->setGeometry(10,10,300,30);
        label_itemText->setStyleSheet("font: 14pt \"Arial\";color: rgb(255, 255, 255);");
        label_itemText->setText(choiceSSID);
        but_save = new QPushButton(widget);
        but_save->setText("保存");
        but_save->setGeometry(270,100,50,30);
        but_save->setStyleSheet("font: 14pt \"黑体\"; color: rgb(51, 153, 255);");
        connect(but_save,SIGNAL(clicked()),this,SLOT(connectNetwork()));
        lineEdit_psk = new QLineEdit(widget);
        lineEdit_psk->setGeometry(10,100,210,30);
        lineEdit_psk->setEchoMode(QLineEdit::Password);//行编辑器内输入的内容为密码输入形式
        lineEdit_psk->setStyleSheet("font: 14pt \"Arial\";");
        but_display = new QPushButton(widget);
        but_display->setGeometry(230,100,30,30);
        but_display->setStyleSheet("border-image: url(:/new/prefix1/view_off2.png);");
        connect(but_display,SIGNAL(pressed()),this,SLOT(mySlots_display_pressed()));
        connect(but_display,SIGNAL(released()),this,SLOT(mySlots_display_released()));
        if(wirelessSSID == choiceSSID){
            lineEdit_psk->setText(wirelessPsk);
        }
        ui->listWidget->setItemWidget(current,widget);
    }
}

void lib_networkManage::connectNetwork()
{
//    qDebug()<<"connectNetwork:"<<ui->listWidget->currentItem()->text();
//    emit sendChoiceSSID(ui->listWidget->currentItem()->text());
    QString ssid = label_itemText->text();
    QString psk = lineEdit_psk->text();
    networkSet.setValue("baseinfo/wirelessSSID",ssid);
    networkSet.setValue("baseinfo/wirelessPsk",psk);
    ui->SSID->setText(ssid);
    ui->psk->setText(psk);
    emit sendChoiceNetwork(ssid,psk);
}

void lib_networkManage::mySlots_display_pressed()
{
    but_display->setStyleSheet("border-image: url(:/new/prefix1/view2.png);");
    lineEdit_psk->setEchoMode(QLineEdit::Normal);
}

void lib_networkManage::mySlots_display_released()
{
    but_display->setStyleSheet("border-image: url(:/new/prefix1/view_off2.png);");
    lineEdit_psk->setEchoMode(QLineEdit::Password);
}

//点击
void lib_networkManage::on_pushButton_pressed()
{
    ui->pushButton->setStyleSheet("border-image: url(:/new/prefix1/view.png);");
    ui->psk->setEchoMode(QLineEdit::Normal);
}

//释放
void lib_networkManage::on_pushButton_released()
{
    ui->pushButton->setStyleSheet("border-image: url(:/new/prefix1/view_off.png);");
    ui->psk->setEchoMode(QLineEdit::Password);
}

//接收setnetwork保存的账号密码
void lib_networkManage::getChoiceNetwork(QString ssid, QString psk)
{
    ui->SSID->setText(ssid);
    ui->psk->setText(psk);
}

//wifi模式
void lib_networkManage::on_raBut_wifi_clicked()
{
    networkSet.setValue("baseinfo/networkMode",0);
    ui->stackedWidget->setCurrentIndex(0);
}

//热点模式
void lib_networkManage::on_raBut_hotspot_clicked()
{
    networkSet.setValue("baseinfo/networkMode",1);
    ui->stackedWidget->setCurrentIndex(1);
    setHotspotIni();
}

//保存热点设置
void lib_networkManage::on_but_save_clicked()
{
    myMsgBox = new myMessageBox;
    myMsgBox->showMsg("保存确认","是否保存所有设置???");
    myMsgBox->show();
    connect(myMsgBox,SIGNAL(sendChoice(bool)),this,SLOT(saveChoice(bool)));
}

//取消保存热点设置
void lib_networkManage::on_but_cancel_clicked()
{
    setHotspotIni();
}

//热点设置初始化
void lib_networkManage::setHotspotIni()
{
    ui->hotspotName->setText(networkSet.value("baseinfo/hotspotName").toString());
    ui->hotspot_psk->setText(networkSet.value("baseinfo/hotspotPsk").toString());
    ui->hotspot_ip->setText(networkSet.value("baseinfo/hotspotIP").toString());
    ui->hotspot_ip_head->setText(networkSet.value("baseinfo/hotspotIP_head").toString());
    ui->hotspot_ip_end->setText(networkSet.value("baseinfo/hotspotIP_end").toString());
    ui->openTime->setText(networkSet.value("baseinfo/hotspotTime").toString());
}

void lib_networkManage::saveChoice(bool cho)
{
    if(cho){
        networkSet.setValue("baseinfo/hotspotName",ui->hotspotName->text());
        networkSet.setValue("baseinfo/hotspotPsk",ui->hotspot_psk->text());
        networkSet.setValue("baseinfo/hotspotIP",ui->hotspot_ip->text());
        networkSet.setValue("baseinfo/hotspotIP_head",ui->hotspot_ip_head->text());
        networkSet.setValue("baseinfo/hotspotIP_end",ui->hotspot_ip_end->text());
        networkSet.setValue("baseinfo/hotspotTime",ui->openTime->text());
    }else{
        setHotspotIni();
    }
    myMsgBox->deleteLater();
}

void lib_networkManage::on_but_viewPsk_pressed()
{
    ui->but_viewPsk->setStyleSheet("border-image: url(:/new/prefix1/view.png);");
    ui->hotspot_psk->setEchoMode(QLineEdit::Normal);
}

void lib_networkManage::on_but_viewPsk_released()
{
    ui->but_viewPsk->setStyleSheet("border-image: url(:/new/prefix1/view_off.png);");
    ui->hotspot_psk->setEchoMode(QLineEdit::Password);
}
