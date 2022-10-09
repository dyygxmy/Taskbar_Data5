#ifndef LIB_MSCONFIG_H
#define LIB_MSCONFIG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QPushButton>
#include <QList>
#include "lib_taskbar_global.h"
#include "mymessagebox.h"

namespace Ui {
class lib_msconfig;
}

class lib_msconfig : public QWidget
{
    Q_OBJECT

public:
    explicit lib_msconfig(QWidget *parent = 0);
    ~lib_msconfig();

private slots:
    void on_but_add_clicked();

    void on_but_subtract_clicked();

    void on_but_save_clicked();

    void setListWidgetValue(bool);
    void getItemRow(int);
    void slot_setUp();
    void slot_setDown();

    void on_but_edit_clicked();

//    void hideSaveResult();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_but_cancel_clicked();

private:
    Ui::lib_msconfig *ui;
    myMessageBox *myMsgBox;
    int itemRow;
    int editMode;//编辑模式
    QString filePath;
    void myAddWidget();
    void setItemRowUp();
    void setItemRowDown();
    typedef struct{
        QWidget *widget;
        QListWidgetItem *item;
        QPushButton *but_up;
        QPushButton *but_down;
        QCheckBox *box;
    }ItemStruct;
    QList<ItemStruct>itemList;
    void setIni();
    bool openEditFlag;//主要限制按钮点击
public slots:
    void closeEdit();
    void openEdit();
    void saveChoice(bool);
};

#endif // LIB_MSCONFIG_H
