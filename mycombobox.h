#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QComboBox>
#include <QMouseEvent>
#include <QWidget>
#include "lib_taskbar_global.h"
#include <QProcess>
class myComboBox : public QComboBox
{
public:
    myComboBox(QWidget*parent=0);
    void showPopup ();
signals:
    void clicked(bool);
private:
    QProcess process;
};

#endif // MYCOMBOBOX_H
