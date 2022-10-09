#ifndef IWCONFIGMSG_H
#define IWCONFIGMSG_H

#include <QObject>
#include <QThread>
#include <QProcess>

class iwconfigMsg : public QObject
{
    Q_OBJECT
public:
    explicit iwconfigMsg(QObject *parent = 0);

signals:
    void sendQuality(QString);
public slots:
    void iwconfigStart();
private slots:
//    void iwconfigRun();

private:
    QThread thread;
    QProcess *process;
    void iwconfigRun();
};

#endif // IWCONFIGMSG_H
