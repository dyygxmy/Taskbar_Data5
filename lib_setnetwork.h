#ifndef LIB_SETNETWORK_H
#define LIB_SETNETWORK_H

#include <QWidget>

#include "mycombobox.h"
#include <QProcess>
#include <QScrollArea>
#include "lib_taskbar_global.h"
#include <QPushButton>
#include <QVBoxLayout>

#include "mymessagebox.h"
#include <QLabel>
#include <QGraphicsOpacityEffect>

namespace Ui {
class lib_setNetwork;
}

class lib_setNetwork : public QWidget
{
    Q_OBJECT

public:
    explicit lib_setNetwork(QWidget *parent = 0);
    ~lib_setNetwork();
    Ui::lib_setNetwork *ui;

private slots:
    void on_but_close_clicked();

    void on_but_save_clicked();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_raBut_wlan0_clicked();

    void on_raBut_eth0_clicked();

    void on_raBut_eth1_clicked();

    void on_but_cancel_clicked();

private:
    myMessageBox *myMsgBox;
    void showConfigInfo();
    void setStyleSheetBatch();//批量设置样式表
    bool openEditFlag;//主要控制按钮允许点击

//    QString bridgingIP;
//    QString bridgingNetwork;
    QString bridgingIPTemp;
    QString bridgingNetworkTemp;
    void setBridging();
public slots:
    void closeEdit();
    void openEdit();
    void saveChoice(bool);
    void getChoiceNetwork(QString,QString);
signals:
//    void sendNTPRun();
//    void sendNTPDATERun(QString);
    void sendChoiceNetwork(QString,QString);
protected:
//    void leaveEvent(QEvent *);
    void focusOutEvent();
};

#endif // LIB_SETNETWORK_H
