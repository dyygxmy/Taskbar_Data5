#ifndef CONNECTCP2110_H
#define CONNECTCP2110_H

#include <QObject>
#include <QThread>

class connectcp2110 : public QObject
{
    Q_OBJECT
public:
    explicit connectcp2110(QObject *parent = 0);

signals:
    void sendCP2110Msg(int);
public slots:
    void startCP2110();
    void readALLCP2110();
    void writeCP2110(int,int);
private:
    QThread thread;
};

#endif // CONNECTCP2110_H
