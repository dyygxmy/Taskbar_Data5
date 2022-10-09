#ifndef MYQAPPLICATION_H
#define MYQAPPLICATION_H
#include <QApplication>
#include "keyboard.h"
#include <QDateTime>
//#include "lib_taskbar_wi.h"
#include "lib_taskbar_wi.h"
class  myQApplication : public QApplication
{
    Q_OBJECT
public:
    myQApplication(int &argc, char **argv);
    ~myQApplication();
    bool notify(QObject *,QEvent *) Q_DECL_OVERRIDE;
public slots:
    void setInputText(QString);
private:
    KeyBoard *keyboard;
    lib_taskbar_wi *lib_wi;
    bool validhits;//一次有效的点击（鼠标按下与释放都在同一个按钮下）
    QString butName_Press;//记录按下时的按钮名
//    QString useKeyboardObj;
    QObject* topParent(QObject*);
//    bool isChildFrom_lib_setNetwork;//lib_setNetwork内的操作
    QStringList windowList;//几个操作界面
    QStringList WidgetWindowList;//几个操作界面的QWidgetWindow
    QStringList objectList_lib_taskbar_wi;//上的操作控件
//    bool FocusOutTo_lib_taskbar_wi;//点击 lib_taskbar_wi 使窗口失去焦点
    bool noHide;//不隐藏窗口的情况
    bool calendarEdit;//日历在输入过程中
    bool noClicked;//不要发clicked事件
    QDateTime timeHide;//记录窗口隐藏的时间
    QDateTime timeFileDialog;//记录准备要跳出QFileDialog窗口的时间
    QDateTime timeMyMsgBox;//记录点击myMessageBox窗口的时间
    bool sendClicked;
};

#endif // MYQAPPLICATION_H
