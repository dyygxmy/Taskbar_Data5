#ifndef SCANWLAN_H
#define SCANWLAN_H

#include <QObject>
#include <QThread>
#include "lib_taskbar_global.h"
#include <QProcess>

class scanWlan : public QObject
{
    Q_OBJECT
public:
    explicit scanWlan(QObject *parent = 0);
    ~ scanWlan();
signals:

public slots:
    void scanStart();
private:
    QThread thread;
    QProcess *process;
signals:
    void sendScanResult(QStringList);
};

#endif // SCANWLAN_H
