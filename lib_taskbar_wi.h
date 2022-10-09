#ifndef LIB_TASKBAR_WI_H
#define LIB_TASKBAR_WI_H

#include <QWidget>
#include "lib_taskbar_global.h"
#include "lib_reoff.h"
#include "lib_setnetwork.h"
#include "lib_iomanage.h"
#include "lib_msconfig.h"
#include "scanwlan.h"
#include <QDesktopWidget>
#include <QProcess>
#include <QProgressBar>
#include <QLabel>
#include "iwconfigmsg.h"
#include "mymessagebox.h"
#include "lib_networkmanage.h"
#include "lib_datetimemanage.h"
#include "ntpmanage.h"
namespace Ui {
class lib_taskbar_wi;
}

class lib_taskbar_wi : public QWidget
{
    Q_OBJECT

public:
    explicit lib_taskbar_wi(QWidget *parent = 0);
    ~lib_taskbar_wi();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void slot_showDateTime();//显示日期时间

    void on_dateTime_clicked();

    void mySetWindow();

//    void winManage();

    void on_but_quy_clicked();

private:
    Ui::lib_taskbar_wi *ui;
    lib_reOFF *reOff;
    lib_setNetwork *setNet;
    lib_IOManage *ioManage;
    lib_msconfig *msconf;
    lib_networkManage *netMng;
    lib_datetimeManage * dateTimeMng;

    ntpManage *ntp;
    QDesktopWidget *desktop;
    QProcess process;
    QList<QWidget*>widgetList;
    void widgetMutex(QWidget*);
    //QProgressBar *bar;
    int type_POWER;//记录实时电源状态
    int va_VOLTAGE;//记录实时电量
    int POWERTypeTemp;//记录本次电源状态
    int VOLTAGEValueTemp;//记录本次电量

    scanWlan scan;
    int clickCount;//点击次数
    bool closeEditFlag;//关闭编辑功能

    iwconfigMsg iwconfig;
//    bool isEventHide;//事件处理的隐藏

//    QStringList objectNameList;
    void sendWinManage(int);
//    int objectIndex;//记录第几个按钮
    void openEdit();
    void closeEdit();

    myMessageBox *myMsgBox;

    int showMyWin;//显示指定的窗口（控制窗口该显示的时候显示）
public slots:
    void getPOWERInfo(int);
    void getVOLTAGEInfo(int);
    void slot_showPower();//显示电源电量
    void slot_showQuality(QString);//显示网络信号
//    void slot_hideType();
    void slot_noClicked();
    void openChoice(bool);
signals:
    void sendCloseEdit();
    void sendOpenEdit();
};

#endif // LIB_TASKBAR_WI_H
