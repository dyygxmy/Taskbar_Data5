#include "lib_datetimemanage.h"
#include "ui_lib_datetimemanage.h"
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include <QTime>


lib_datetimeManage::lib_datetimeManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_datetimeManage)
{
    ui->setupUi(this);
//    ui->dateTimeEdit->setCalendarPopup(true);//日历弹出显示模式
    QString ntpServer = networkSet.value("baseinfo/ntpServer").toString();
    ui->ntpServer->setText(ntpServer);
//    ui->dateEdit->setFocusPolicy(Qt::NoFocus);
//    ui->timeEdit->setFocusPolicy(Qt::NoFocus);
//    ui->dateEdit->setDate(QDate::currentDate());
//    ui->timeEdit->setTime(QTime::currentTime());
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    int year = date.year();
    int month = date.month();
    int day = date.day();
    int hour = time.hour();
    int minute = time.minute();
    int second = time.second();
    ui->year->setText(QString::number(year));
    if(month > 9){
        ui->month->setText(QString::number(month));
    }else{
        ui->month->setText("0"+QString::number(month));
    }
    if(day > 9){
        ui->day->setText(QString::number(day));
    }else{
        ui->day->setText("0"+QString::number(day));
    }
    if(hour > 9){
        ui->hour->setText(QString::number(hour));
    }else{
        ui->hour->setText("0"+QString::number(hour));
    }
    if(minute > 9){
        ui->minute->setText(QString::number(minute));
    }else{
        ui->minute->setText("0"+QString::number(minute));
    }
    if(second > 9){
        ui->second->setText(QString::number(second));
    }else{
        ui->second->setText("0"+QString::number(second));
    }

    //有配置就取配置，没配置就配置一个给个默认值 开启NTP功能
    if(networkSet.contains("baseinfo/CalibrationON")){
        GOpenNTP = networkSet.value("baseinfo/OpenNTP").toBool();
    }else{
        GOpenNTP = false;
        networkSet.setValue("baseinfo/OpenNTP",GOpenNTP);
    }
    if(GOpenNTP){
        ui->raBut_runNTP->clicked(true);
        emit sendNTPRun();
    }else{
        ui->raBut_runNTPDATE->clicked(true);
        emit sendNTPDATERun(ui->ntpServer->text());
    }
}

lib_datetimeManage::~lib_datetimeManage()
{
    delete ui;
}

void lib_datetimeManage::on_update_clicked()
{
    QString setDatetime = ui->year->text() + ui->month->text() + ui->day->text() + " " + ui->hour->text() + ":" + ui->minute->text() + ":" + ui->second->text();
//    qDebug()<<"this datetime:"<<setDatetime;
    QString updateBash = "date -s '" + setDatetime + "'";
    system(updateBash.toLocal8Bit().data());
    system("hwclock -w");
}

void lib_datetimeManage::on_save_clicked()
{
    networkSet.setValue("baseinfo/ntpServer",ui->ntpServer->text());
}

//外网同步（ntp服务自动同步）
//void lib_datetimeManage::on_but_openNTP_clicked()
//{
//    if(!GOpenNTP){
//        GOpenNTP = true;
//        ui->but_openNTPDATE->setStyleSheet("font: 14pt \"黑体\";background-color: rgb(255, 255, 255);color: rgb(51, 153, 255);border-width:1px;border-style:solid;border-color:rgb(51, 153, 255);");
//        ui->but_openNTP->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/67.bmp);background:transparent;font: 14pt \"黑体\";color: rgb(248, 248, 255);");
//        networkSet.setValue("baseinfo/OpenNTP",GOpenNTP);
//    }
//    emit sendNTPRun();
//}

//内网同步（ntpdate工具从服务器上获取时间）
//void lib_datetimeManage::on_but_openNTPDATE_clicked()
//{
//    if(GOpenNTP){
//        GOpenNTP = false;
//        ui->but_openNTP->setStyleSheet("font: 14pt \"黑体\";background-color: rgb(255, 255, 255);color: rgb(51, 153, 255);border-width:1px;border-style:solid;border-color:rgb(51, 153, 255);");
//        ui->but_openNTPDATE->setStyleSheet("border-image: url(:/prefix2/LCD_CS351/35_all/67.bmp);background:transparent;font: 14pt \"黑体\";color: rgb(248, 248, 255);");
//        networkSet.setValue("baseinfo/OpenNTP",GOpenNTP);
//    }
//    emit sendNTPDATERun(ui->ntpServer->text());
//}

void lib_datetimeManage::on_getCurrentDatetime_clicked()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    int year = date.year();
    int month = date.month();
    int day = date.day();
    int hour = time.hour();
    int minute = time.minute();
    int second = time.second();
    ui->year->setText(QString::number(year));
    if(month > 9){
        ui->month->setText(QString::number(month));
    }else{
        ui->month->setText("0"+QString::number(month));
    }
    if(day > 9){
        ui->day->setText(QString::number(day));
    }else{
        ui->day->setText("0"+QString::number(day));
    }
    if(hour > 9){
        ui->hour->setText(QString::number(hour));
    }else{
        ui->hour->setText("0"+QString::number(hour));
    }
    if(minute > 9){
        ui->minute->setText(QString::number(minute));
    }else{
        ui->minute->setText("0"+QString::number(minute));
    }
    if(second > 9){
        ui->second->setText(QString::number(second));
    }else{
        ui->second->setText("0"+QString::number(second));
    }
}

void lib_datetimeManage::on_but_yearAdd_clicked()
{
    ui->year->setText(QString::number(ui->year->text().toInt() + 1));
    checkOutDays();
}

void lib_datetimeManage::on_but_yearSub_clicked()
{
    ui->year->setText(QString::number(ui->year->text().toInt() - 1));
    checkOutDays();
}

void lib_datetimeManage::on_but_monthAdd_clicked()
{
    int month = (ui->month->text().toInt() + 1)%13;
    if(0 == month){
        month = 1;
    }
    if(month > 9){
        ui->month->setText(QString::number(month));
    }else{
        ui->month->setText("0"+QString::number(month));
    }
    checkOutDays();
}

void lib_datetimeManage::on_but_monthSub_clicked()
{
    int month = (ui->month->text().toInt() - 1)%13;
    if(1 > month){
        month = 12;
    }
    if(month > 9){
        ui->month->setText(QString::number(month));
    }else{
        ui->month->setText("0"+QString::number(month));
    }
    checkOutDays();
}

void lib_datetimeManage::on_but_dayAdd_clicked()
{
    int year = ui->year->text().toInt();
    int month = ui->month->text().toInt();
    int maxDays = getMaxDays(year,month);

    int date = (ui->day->text().toInt() + 1)%(maxDays+1);
    if(0 == date){
        date = 1;
    }
    if(date > 9){
        ui->day->setText(QString::number(date));
    }else{
        ui->day->setText("0"+QString::number(date));
    }
}

void lib_datetimeManage::on_but_daySub_clicked()
{
    int year = ui->year->text().toInt();
    int month = ui->month->text().toInt();
    int maxDays = getMaxDays(year,month);
    int date = (ui->day->text().toInt() - 1)%(maxDays+1);
    if(1 > date){
        date = maxDays;
    }
    if(date > 9){
        ui->day->setText(QString::number(date));
    }else{
        ui->day->setText("0"+QString::number(date));
    }
}

void lib_datetimeManage::on_but_hourAdd_clicked()
{
    int hour = (ui->hour->text().toInt() + 1)%24;
    if(hour > 9){
        ui->hour->setText(QString::number(hour));
    }else{
        ui->hour->setText("0"+QString::number(hour));
    }
}

void lib_datetimeManage::on_but_hourSub_clicked()
{
    int hour = (ui->hour->text().toInt() - 1)%24;
    if(0 > hour){
        hour = 23;
    }
    if(hour > 9){
        ui->hour->setText(QString::number(hour));
    }else{
        ui->hour->setText("0"+QString::number(hour));
    }
}

void lib_datetimeManage::on_but_minuteAdd_clicked()
{
    int minute = (ui->minute->text().toInt() + 1)%60;
    if(minute > 9){
        ui->minute->setText(QString::number(minute));
    }else{
        ui->minute->setText("0"+QString::number(minute));
    }
}

void lib_datetimeManage::on_but_minuteSub_clicked()
{
    int minute = (ui->minute->text().toInt() - 1)%60;
    if(0 > minute){
        minute = 59;
    }
    if(minute > 9){
        ui->minute->setText(QString::number(minute));
    }else{
        ui->minute->setText("0"+QString::number(minute));
    }
}

void lib_datetimeManage::on_but_secondAdd_clicked()
{
    int second = (ui->second->text().toInt() + 1)%60;
    if(second > 9){
        ui->second->setText(QString::number(second));
    }else{
        ui->second->setText("0"+QString::number(second));
    }
}

void lib_datetimeManage::on_but_secondSub_clicked()
{
    int second = (ui->second->text().toInt() - 1)%60;
    if(0 > second){
        second = 59;
    }
    if(second > 9){
        ui->second->setText(QString::number(second));
    }else{
        ui->second->setText("0"+QString::number(second));
    }
}


int lib_datetimeManage::getMaxDays(int year, int month)
{
    int maxDays = 28;
    //计算闰年
    if((0 == year%4 && 0 != year%100) || 0 == year%400){
        if(2 == ui->month->text().toInt()){
            maxDays = 29;
        }
    }else{//计算非闰年
        if(2 == ui->month->text().toInt()){
            maxDays = 28;
        }
    }
    if(1 == month || 3 == month || 5 == month || 7 == month || 8 == month || 10 == month || 12 == month){
        maxDays = 31;
    }
    if(4 == month || 6 == month || 9 == month || 11 == month){
        maxDays = 30;
    }
    return maxDays;
}

void lib_datetimeManage::checkOutDays()
{
    int year_current = ui->year->text().toInt();
    int month_current = ui->month->text().toInt();
    int day_current = ui->day->text().toInt();
    int maxDays = getMaxDays(year_current,month_current);
    if(day_current > maxDays){
        ui->day->setText(QString::number(maxDays));
    }
}

//内网同步（ntpdate工具从服务器上获取时间）
void lib_datetimeManage::on_raBut_runNTPDATE_clicked()
{
    if(GOpenNTP){
        GOpenNTP = false;
        ui->raBut_runNTPDATE->clicked(true);
        networkSet.setValue("baseinfo/OpenNTP",GOpenNTP);
    }
    emit sendNTPDATERun(ui->ntpServer->text());
}

//外网同步（ntp服务自动同步）
void lib_datetimeManage::on_raBut_runNTP_clicked()
{
    if(!GOpenNTP){
        GOpenNTP = true;
        ui->raBut_runNTP->clicked(true);
        networkSet.setValue("baseinfo/OpenNTP",GOpenNTP);
    }
    emit sendNTPRun();
}
