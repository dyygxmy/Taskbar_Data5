#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QDialog>
#include <QDesktopWidget>
#include "lib_taskbar_global.h"

namespace Ui {
class myMessageBox;
}

class myMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit myMessageBox(QWidget *parent = 0);
    ~myMessageBox();
    QString msg;
    void showMsg(QString,QString);
private slots:
    void on_but_yes_clicked();

    void on_but_no_clicked();

    void on_but_cancel_clicked();

signals:
    void sendChoice(bool);

private:
    Ui::myMessageBox *ui;
    QDesktopWidget *desktop;
protected:
    void paintEvent(QPaintEvent*);
};

#endif // MYMESSAGEBOX_H
