#include "hotspotmanage.h"
#include "unistd.h"

//***未使用，热点功能直接写到python脚本中去

hotspotManage::hotspotManage(QObject *parent) : QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
}

void hotspotManage::openHotspot()
{
    system("killall wpa_supplicant");
    system("ap-hotspot start");
}


void hotspotManage::openWifi()
{
    system("ap-hotspot stop");
    system("wpa_supplicant -B -P /var/run/wpa_supplicant.wlan0.pid -i wlan0 -c /etc/dat5/myPythonProject+hide/wpa/wpa_supplicant.conf");
}
