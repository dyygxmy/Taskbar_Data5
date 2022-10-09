#ifndef LIB_TASKBAR_GLOBAL_H
#define LIB_TASKBAR_GLOBAL_H
#include <QSettings>
#define DYTEST0 1  //显示光标，打印
#define DYTEST1 0  //scan list结果添加值
#define DYTEST3 0  //手动给研华IO发指令

#define OPENYANHUAIO 1 //研华的IO控制 1打开 0关闭

#define USECP2110 1 //使用CP2110


#include <QtCore/qglobal.h>

static bool GOpenNTP = false;
static QString DNSPath = "/etc/resolvconf/resolv.conf.d/base";
static QSettings networkSet("/allMyConfig/network.ini",QSettings::IniFormat);
static QString thisVersion = "V20.8.6.1";
static QString GConnectGateway = "wlan0";

#endif // LIB_TASKBAR_GLOBAL_H
