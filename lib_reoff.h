#ifndef LIB_REOFF_H
#define LIB_REOFF_H

#include <QWidget>

#include <QProcess>
#include "lib_taskbar_global.h"

namespace Ui {
class lib_reOFF;
}

class lib_reOFF : public QWidget
{
    Q_OBJECT

public:
    explicit lib_reOFF(QWidget *parent = 0);
    ~lib_reOFF();
    void rebootFunc();
    void poweriffFunc();
    void setIni();

private slots:
    void on_but_reboot_clicked();

    void on_but_poweroff_clicked();

private:
    Ui::lib_reOFF *ui;
    QProcess process;
public slots:
    void closeEdit();
    void openEdit();
protected:
    void paintEvent(QPaintEvent*);
};

#endif // LIB_REOFF_H
