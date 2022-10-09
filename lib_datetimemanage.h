#ifndef LIB_DATETIMEMANAGE_H
#define LIB_DATETIMEMANAGE_H

#include <QWidget>
#include "lib_taskbar_global.h"

namespace Ui {
class lib_datetimeManage;
}

class lib_datetimeManage : public QWidget
{
    Q_OBJECT

public:
    explicit lib_datetimeManage(QWidget *parent = 0);
    ~lib_datetimeManage();

private slots:
    void on_update_clicked();

    void on_save_clicked();

//    void on_but_openNTP_clicked();

//    void on_but_openNTPDATE_clicked();

    void on_getCurrentDatetime_clicked();

    void on_but_yearAdd_clicked();

    void on_but_yearSub_clicked();

    void on_but_monthAdd_clicked();

    void on_but_monthSub_clicked();

    void on_but_dayAdd_clicked();

    void on_but_daySub_clicked();

    void on_but_hourAdd_clicked();

    void on_but_hourSub_clicked();

    void on_but_minuteAdd_clicked();

    void on_but_minuteSub_clicked();

    void on_but_secondAdd_clicked();

    void on_but_secondSub_clicked();

    void on_raBut_runNTPDATE_clicked();

    void on_raBut_runNTP_clicked();

private:
    Ui::lib_datetimeManage *ui;
    int getMaxDays(int,int);
    void checkOutDays();
signals:
    void sendNTPRun();
    void sendNTPDATERun(QString);
};

#endif // LIB_DATETIMEMANAGE_H
