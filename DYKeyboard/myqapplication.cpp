#include "myqapplication.h"
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>

myQApplication::myQApplication(int &argc, char **argv)
    :QApplication(argc,argv)
{
    keyboard = new KeyBoard;
    connect(keyboard,SIGNAL(characterGenerated(QString)),this,SLOT(setInputText(QString)));
//    isChildFrom_lib_setNetwork = false;//lib_setNetwork内的操作
    windowList<<"lib_setNetwork"<<"lib_reOFF"<<"lib_IOManage"<<"lib_msconfig"<<"lib_networkManage"<<"lib_datetimeManage";
    WidgetWindowList<<"lib_setNetworkWindow"<<"lib_reOFFWindow"<<"lib_IOManageWindow"<<"lib_msconfigWindow"<<"lib_networkManageWindow"<<"lib_datetimeManageWindow";
    objectList_lib_taskbar_wi<<"pushButton"<<"pushButton_2"<<"pushButton_3"<<"pushButton_4"<<"but_quy"<<"dateTime";
    timeHide = QDateTime::currentDateTime();//控制界面隐藏时触发的clicked信号不会跳出界面
    timeFileDialog = QDateTime::currentDateTime();//控制在lib_msconfig界面上点but_add跳出QFileDialog时不隐藏窗口
    timeMyMsgBox = QDateTime::currentDateTime();
    noHide = false;//不隐藏窗口的情况
    calendarEdit = false;//日历在输入过程中
    noClicked = false;//不要发clicked事件
    sendClicked = true;
    validhits = true;//一次有效的点击（鼠标按下与释放都在同一个按钮下）
    butName_Press = "";//记录按下时的按钮名
}

myQApplication::~myQApplication()
{
    keyboard->deleteLater();
}
bool myQApplication::notify(QObject *watched, QEvent *event)
{
//        qDebug()<<"eventFilter:"<<watched<<event->type();
    //点击("SSID")出现的事件 MouseMove-ToolTip-MouseButtonPress-FocusIn-Paint-MouseButtonRelease
    //移出("SSID")点击别处出现的事件 MouseMove-MouseMove(外面控件事件)-Leave-Paint-MouseButtonPress(外面控件事件)-FocusAboutToChange-FocusOut

    if(event->type() == QEvent::MouseButtonPress){//鼠标按下
//        qDebug()<<"MouseButtonPress:"<<watched<<topParent(watched)<<QApplication::activeWindow();
        //点击空白处，键盘界面隐藏
        if(
                !QString(watched->metaObject()->className()).contains("Edit") //非编辑类隐藏键盘
                && topParent(watched)->objectName() != keyboard->objectName()//键盘界面上点击不隐藏
                && watched->objectName() != "KeyBoardWindow" //忽略QWidgetWindow(name = "KeyBoardWindow")的事件
                ){
            if(QApplication::activeWindow()){
                QApplication::activeWindow()->setFocus(Qt::MenuBarFocusReason);//菜单栏成了焦点
            }
            keyboard->hide();
        }

        if(watched->objectName() == "KeyBoardWindow"){//键盘点击导致原窗口失去焦点不隐藏
            noHide = true;
        }

        //本次隐藏窗口是在点击指定按钮区域上
        int tidi = timeHide.msecsTo(QDateTime::currentDateTime());//测试时发现有191的情况
//        qDebug()<<"send noClick:"<<tidi;
        if(objectList_lib_taskbar_wi.contains(watched->objectName())){
            if(noClicked && tidi < 500){
                noClicked = false;
                //            QTimer::singleShot(0,topParent(watched),SLOT(slot_noClicked()));//使本次clicked事件不生效 用信号槽比较慢，如果在clicked事件已经触发后就不能有效控制了
                lib_wi = qobject_cast<lib_taskbar_wi*>(topParent(watched));
                lib_wi->slot_noClicked();//直接调用函数比较快
            }
            butName_Press = watched->objectName();
        }
        if(QApplication::activeWindow()){
            if(topParent(watched)->objectName() == "myMessageBox" && QApplication::activeWindow()->objectName() != "myMessageBox"){
                timeMyMsgBox = QDateTime::currentDateTime();
            }
        }
    }else if (event->type()==QEvent::FocusIn){//获得键盘焦点
//        qDebug()<<"FocusIn:"<<watched<<topParent(watched)<<QApplication::activeWindow();
        if(QApplication::activeWindow() && watched && watched->parent()){
            if(
                    QString(watched->parent()->metaObject()->className()) == "QFileDialog"&&
                    QString(watched->metaObject()->className()) == "QLineEdit"
                    ){
                qobject_cast<QLineEdit*>(watched)->hide();//隐藏QFileDialog窗口中的QLineEdit
            }
        }

        if(
                QString(watched->metaObject()->className()).contains("Edit")//编辑类才打开虚拟键盘
                && QString(watched->parent()->metaObject()->className()) != "QFileDialog"//QFileDialog窗口不使用keyboard
                ){
            noHide = true;//刚在处理编辑不隐藏
            if(keyboard->isHidden()){
                keyboard->show();
            }
        }
        if(watched->objectName() == "KeyBoard"){
            noHide = false;//焦点已经换到键盘，允许处理隐藏了
        }
//        qDebug()<<"FocusIn end";
    }else if(event->type() == QEvent::MouseButtonRelease){//鼠标释放
//        qDebug()<<"MouseButtonRelease:"<<watched<<topParent(watched)<<QApplication::activeWindow();
        if(watched->objectName() == "KeyBoardWindow"){
            noHide = false;//刚才有在键盘界面释放鼠标，允许处理隐藏了
        }

        if(watched->objectName() == "lib_reOFFWindow" && QApplication::activeWindow()->objectName() == "lib_reOFF"){
            QApplication::activeWindow()->hide();
        }
        if(watched->objectName() == "but_add" && topParent(watched)->objectName() == "lib_msconfig"){
            timeFileDialog = QDateTime::currentDateTime();//释放鼠标时，如果点击的是lib_msconfig界面的but_add按钮，记录时间
        }
        //打开QPushButton点击事件(放在最后面，不然发送clicked事件时触发其他事件会导致释放事件后面的代码不能执行)
        if(sendClicked && QString(watched->metaObject()->className()) == "QPushButton"){
            if(QApplication::activeWindow()){
                QApplication::activeWindow()->setFocus(Qt::MenuBarFocusReason);//菜单栏成了焦点 不加这句话点搜索按钮焦点会跑到lineEdit上去
            }
            QPushButton* but = qobject_cast<QPushButton*>(watched);
//            emit but->click();//触发QPushButton点击事件，因为重写了，不加这句QPushButton信号槽无法触发
//            emit but->clicked();
            if(objectList_lib_taskbar_wi.contains(watched->objectName())){
//                qDebug()<<"setEnabled false"<<butName_Press;
                if(butName_Press != watched->objectName()){
                    lib_wi = qobject_cast<lib_taskbar_wi*>(topParent(watched));
                    lib_wi->slot_noClicked();
                }
            }
//            qDebug()<<"butName_Press clear"<<butName_Press;
//            but->clicked();//不能用这个，不然会触发2次clicked信号
            but->click();
            butName_Press = "";
        }
    }else if(event->type()==QEvent::FocusOut){//失去键盘焦点
//        qDebug()<<"FocusOut:"<<watched<<topParent(watched)<<QApplication::activeWindow();
        if(QApplication::activeWindow()){
            if(
                    windowList.contains(QApplication::activeWindow()->objectName())//失去焦点的时的活动窗口是指定窗口
                    && windowList.indexOf(QApplication::activeWindow()->objectName()) == WidgetWindowList.indexOf(watched->objectName())
                    ){
                int tidi = timeFileDialog.msecsTo(QDateTime::currentDateTime());
                int tidi2 = timeMyMsgBox.msecsTo(QDateTime::currentDateTime());
//                qDebug()<<"hide windows[000]"<<tidi2;
                if(!noHide && !calendarEdit && tidi > 1000 && tidi2 > 500){//测试点击but_add按钮到lib_msconfig窗口失去焦点大概160~310ms，而到关掉需要隐藏窗口时最少要1600ms
//                    qDebug()<<"hide windows[0]"<<tidi2;
                    QApplication::activeWindow()->hide();
                    keyboard->hide();
                    noClicked = true;
                    timeHide = QDateTime::currentDateTime();
                }
                noHide = false;
            }
        }else{
            if(
                    QString(watched->metaObject()->className()).contains("Edit")//防止在操作键盘输入的时候隐藏
                    ){
                noHide = true;
            }
        }
    }
    //eventFilter的实现的最后必须调用watcher基类的eventFilter函数以传递事件。 如果不调的话watcher的所有事件都将丢失
    return QApplication::notify(watched,event);
}

void myQApplication::setInputText( QString c )
{
//    qDebug()<<"setInputText:"<<c;

    if(c == "Esc"){
        QApplication::activeWindow()->setFocus(Qt::MenuBarFocusReason);//菜单栏成了焦点
        keyboard->hide();
    }else{
        QPointer<QWidget> w = QApplication::focusWidget();
        if (!w)
            return;

        /* 当收到按键面板的按键输入后，分别向当前焦点Widget发送KeyPress和KeyRelease事件 */
        //c.unicode()
        QKeyEvent keyPress(QEvent::KeyPress, c.toUShort(), Qt::NoModifier, c);
        QApplication::sendEvent(w, &keyPress);

        if (!w)
            return;
        QKeyEvent keyRelease(QEvent::KeyRelease, c.toUShort(), Qt::NoModifier, c);
        QApplication::sendEvent(w, &keyRelease);
    }
}

QObject* myQApplication::topParent(QObject * obj)
{
    while(obj->parent()){
        obj = obj->parent();
    }
    return obj;
}
