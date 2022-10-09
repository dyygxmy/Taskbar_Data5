#ifndef LIB_NETWORKMANAGE_H
#define LIB_NETWORKMANAGE_H

#include <QWidget>
#include <QScrollBar>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QListWidgetItem>
#include "mymessagebox.h"
#include "lib_taskbar_global.h"
namespace Ui {
class lib_networkManage;
}

class lib_networkManage : public QWidget
{
    Q_OBJECT

public:
    explicit lib_networkManage(QWidget *parent = 0);
    ~lib_networkManage();

private:
    Ui::lib_networkManage *ui;
    QScrollBar * scrollBar;
    QWidget * widget;
    QPushButton * but_save;
    QPushButton * but_display;
    QLabel * label_itemText;
    QLineEdit * lineEdit_psk;
    QString wirelessSSID;
    QString wirelessPsk;
    QString choiceSSID;
    myMessageBox *myMsgBox;
    int networkMode;//
    void setHotspotIni();
public slots:
    void scanWlanStart();
    void getScanList(QStringList);
    void getChoiceNetwork(QString,QString);
signals:
    void sendScanWlan();
    void sendChoiceNetwork(QString,QString);
private slots:
    void connectNetwork();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void mySlots_display_pressed();
    void mySlots_display_released();
    void on_pushButton_pressed();
    void on_pushButton_released();
    void on_raBut_wifi_clicked();
    void on_raBut_hotspot_clicked();
    void on_but_save_clicked();
    void on_but_cancel_clicked();
    void saveChoice(bool);
    void on_but_viewPsk_pressed();
    void on_but_viewPsk_released();
};

#endif // LIB_NETWORKMANAGE_H
