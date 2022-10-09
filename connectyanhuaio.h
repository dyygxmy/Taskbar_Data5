#ifndef CONNECTYANHUAIO_H
#define CONNECTYANHUAIO_H

#include <QObject>
#include <QThread>
#include "gpio_manage/Susi4.h"
//#include <QReadWriteLock>//用这鸡毛界面卡的很

class connectYanhuaIO : public QObject
{
    Q_OBJECT
public:
    explicit connectYanhuaIO(QObject *parent = 0);

signals:
    void sendYanhuaIOMsg(int);
private slots:
    void connecttManage();

public slots:
    void connectStart();
    void getSetIO(int,int,int,int);
private:
    QThread thread;
    uint32_t pvalue;
    uint32_t pvalueTemp;
//    QReadWriteLock lock;
    void writeIO(int,int,int);
    bool isWriteHandle;//向IO写数据
    int iPin;
    int type;
    int value;
};

#endif // CONNECTYANHUAIO_H
