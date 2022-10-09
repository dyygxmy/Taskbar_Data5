#ifndef NTPMANAGE_H
#define NTPMANAGE_H

#include <QObject>
#include <QThread>

class ntpManage : public QObject
{
    Q_OBJECT
public:
    explicit ntpManage(QObject *parent = 0);

signals:

public slots:
    void ntpRun();
    void ntpdateRun(QString);
private:
    QThread thread;
};

#endif // NTPMANAGE_H
