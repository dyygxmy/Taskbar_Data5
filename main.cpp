#include "mainwindow.h"
#include <QApplication>
#include "lib_taskbar_wi.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QTimer>
#include <QTime>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QSettings>
//#include <QCoreApplication>
#include "DYKeyboard/myqapplication.h"

/******************日志打印级别函数****************************/
void outputMessage(QtMsgType type,const QMessageLogContext &context,const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text = "";
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
        abort();
    }
    if(text != QString("Warning:")){
        QString message = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(text).arg(msg);
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("yyyy-MM-dd");

        QFile file(QString("/log/")+date+QString("_taskBar.log"));
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << endl;
        file.flush();
        file.close();
    }
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
//    QApplication a(argc, argv);

    myQApplication a(argc,argv);
#if DYTEST0^1
    qInstallMessageHandler(outputMessage);//重定向日志到文件
    QApplication::setOverrideCursor(Qt::BlankCursor);//去掉光标
#endif
    qDebug()<<"thisVersion:"<<thisVersion;
    QProcess pro;
    pro.start("pwd");
    pro.waitForFinished();
    qDebug()<<"currentPath:"<<QDir::currentPath();
    qDebug()<<"pwd:"<<pro.readAllStandardOutput();
    qDebug()<<"applicationDirPath:"<<QCoreApplication::applicationDirPath();
    lib_taskbar_wi wi;
    wi.show();
    return a.exec();
}
