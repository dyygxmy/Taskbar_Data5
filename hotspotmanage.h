#ifndef HOTSPOTMANAGE_H
#define HOTSPOTMANAGE_H

#include <QObject>
#include <QThread>

class hotspotManage : public QObject
{
    Q_OBJECT
public:
    explicit hotspotManage(QObject *parent = 0);

signals:

public slots:
    void openHotspot();
    void openWifi();
private:
    QThread thread;
};

#endif // HOTSPOTMANAGE_H
