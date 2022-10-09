#include "lib_msconfig.h"
#include "ui_lib_msconfig.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTimer>
//#include "../../DWKeyboard/include/KeyboardGlobal.h"
lib_msconfig::lib_msconfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lib_msconfig)
{
    ui->setupUi(this);
    ui->lineEdit->setFocusPolicy(Qt::ClickFocus);
    this->setFocusPolicy(Qt::ClickFocus);
//    GlobalInit();
//    ui->lineEdit->installEventFilter(this);
    itemRow = 0;
    openEditFlag = true;//主要限制按钮点击
//    ui->label->setText("");
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    filePath = QDir::homePath() + "/.config/lxsession/Lubuntu/autostart";
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(getItemRow(int)));
    setIni();
    editMode = 0;//编辑模式
}

lib_msconfig::~lib_msconfig()
{
    delete ui;
}
//添加item
void lib_msconfig::on_but_add_clicked()
{
//    ui->label->setText("");
    if(openEditFlag){
        QString curPath = "/";//初始目录
        QString dlgTitle = "选择文件";//窗口标题
        QString filter = "所有文件(*.*);;文本文件(.txt);;shell(.sh);;python(.py)";//文件格式筛选
        QFileDialog fileDialog;
        //    fileDialog.setContextMenuPolicy(Qt::NoContextMenu);
        QString getOpenFileName = fileDialog.getOpenFileName(this,dlgTitle,curPath,filter);
        ui->lineEdit->setText(getOpenFileName);
        if(getOpenFileName.size() > 0){
            ui->lineEdit->clear();
            ui->listWidget->addItem(getOpenFileName);
            myAddWidget();
        }
    }
}
//删除item
void lib_msconfig::on_but_subtract_clicked()
{
//    ui->label->setText("");
    if(openEditFlag){
        int currentRow = ui->listWidget->currentRow();
        itemList.removeAt(currentRow);//删除当前行
        ui->listWidget->takeItem(currentRow);//删除当前行
    }
}

//保存
void lib_msconfig::on_but_save_clicked()
{
    if(openEditFlag){
        myMsgBox = new myMessageBox;
        myMsgBox->showMsg("保存确认","是否保存所有设置???");
        myMsgBox->show();
        connect(myMsgBox,SIGNAL(sendChoice(bool)),this,SLOT(saveChoice(bool)));
    }

//    ui->label->setText("");
//    QFile startFile(filePath);
//    if(startFile.open(QFile::WriteOnly)){
//        QTextStream in(&startFile);
//        for(int i = 0;i < ui->listWidget->count();i++){
//            in<<ui->listWidget->item(i)->text()<<endl;
////            qDebug()<<"save:"<<ui->listWidget->item(i)->text();
//        }
//        ui->label->setText("保存成功");
//        startFile.close();
//    }else{
//        ui->label->setText("保存失败");
//        qDebug()<<"write open file:"<<startFile.errorString();
//    }
//    QTimer::singleShot(3*1000,this,SLOT(hideSaveResult()));
}

void lib_msconfig::saveChoice(bool cho)
{
    if(cho){
        QFile startFile(filePath);
        if(startFile.open(QFile::WriteOnly)){
            QTextStream in(&startFile);
            for(int i = 0;i < ui->listWidget->count();i++){
                in<<ui->listWidget->item(i)->text()<<endl;
            }
            startFile.close();
        }else{
            qDebug()<<"write open file:"<<startFile.errorString();
        }
    }else{
        setIni();
    }
    myMsgBox->deleteLater();
}

//void lib_msconfig::hideSaveResult()
//{
//    ui->label->setText("");
//}

//修改当前行的值
void lib_msconfig::setListWidgetValue(bool state)
{
    QCheckBox *box = (QCheckBox*)sender();
    for(int i = 0;i < itemList.size();i++){
        if(box == itemList.at(i).box){
            itemRow = i;
            ui->listWidget->setCurrentRow(itemRow);
            break;
        }
    }
    QString va = ui->listWidget->currentItem()->text();
    if(state){//勾选
        if("#" == va.mid(0,1)){
            ui->listWidget->currentItem()->setText(va.right(va.size()-1));
        }
    }else{//取消勾选
        if("#" != va.mid(0,1)){
            ui->listWidget->currentItem()->setText("#"+va);
        }
    }
}

//获取当前在操作的行
void lib_msconfig::getItemRow(int row)
{
    itemRow = row;
//    ui->listWidget->item(row)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserTristate);
}

//设置item上移
void lib_msconfig::setItemRowUp()
{
    QPushButton *but_up = (QPushButton*)sender();
    int itemCount = itemList.size();
    //根据but_up确认点击的行号
    for(int i = 0;i < itemCount;i++){
        if(but_up == itemList.at(i).but_up){
            itemRow = i;
            break;
        }
    }
    if(itemRow > 0){
        //交换listWidget中item的文本
        QString va = ui->listWidget->item(itemRow)->text();
        ui->listWidget->item(itemRow)->setText(ui->listWidget->item(itemRow - 1)->text());
        ui->listWidget->item(itemRow - 1)->setText(va);
        itemRow--;
        //活动项变化
        ui->listWidget->setCurrentRow(itemRow);
    }
    //文本有变化，重新确定box的勾选
    for(int i = 0;i<itemCount;i++){
        QString itemText = ui->listWidget->item(i)->text();
        ItemStruct addItem = itemList.at(i);
        if("#" == itemText.mid(0,1)){
            addItem.box->setChecked(false);
        }else{
            addItem.box->setChecked(true);
        }
    }
}


//设置item下移
void lib_msconfig::setItemRowDown()
{
    QPushButton *but_down = (QPushButton*)sender();
    int itemCount = itemList.size();
    //根据but_up确认点击的行号
    for(int i = 0;i < itemCount;i++){
        if(but_down == itemList.at(i).but_down){
            itemRow = i;
            break;
        }
    }
    if(itemRow < ui->listWidget->count() - 1){
        //交换listWidget中item的文本
        QString va = ui->listWidget->item(itemRow)->text();
        ui->listWidget->item(itemRow)->setText(ui->listWidget->item(itemRow + 1)->text());
        ui->listWidget->item(itemRow + 1)->setText(va);
        itemRow++;
        //活动项变化
        ui->listWidget->setCurrentRow(itemRow);
    }
    //文本有变化，重新确定box的勾选
    for(int i = 0;i<itemCount;i++){
        QString itemText = ui->listWidget->item(i)->text();
        ItemStruct addItem = itemList.at(i);
        if("#" == itemText.mid(0,1)){
            addItem.box->setChecked(false);
        }else{
            addItem.box->setChecked(true);
        }
    }
}

//添加QWidget，并在QWidget中添加多功能控件，与item对齐
void lib_msconfig::myAddWidget()
{
    ItemStruct addItem;
    addItem.widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *but_up = new QPushButton(addItem.widget);
    QPushButton *but_down = new QPushButton(addItem.widget);
    QCheckBox *box = new QCheckBox(addItem.widget);
    addItem.item = ui->listWidget->item(ui->listWidget->count() - 1);
    addItem.but_up = but_up;
    addItem.but_down = but_down;
    addItem.box = box;
    itemList<<addItem;
    but_up->setText("↑");
    but_down->setText("↓");
    but_up->setFlat(true);
    but_down->setFlat(true);
    if("#" == addItem.item->text().mid(0,1)){
        box->setChecked(false);
    }else{
        box->setChecked(true);
    }
    connect(box,SIGNAL(clicked(bool)),this,SLOT(setListWidgetValue(bool)));
    connect(but_up,SIGNAL(clicked()),this,SLOT(slot_setUp()));
    connect(but_down,SIGNAL(clicked()),this,SLOT(slot_setDown()));
    but_up->setGeometry(ui->listWidget->width()-35-35-35,0,35,35);
    but_down->setGeometry(ui->listWidget->width()-35-35,0,35,35);
    box->setGeometry(ui->listWidget->width()-35,0,35,35);
    layout->addWidget(addItem.widget);
    ui->listWidget->setItemWidget(ui->listWidget->item(ui->listWidget->count()-1),addItem.widget);
}


void lib_msconfig::slot_setUp()
{
    setItemRowUp();
}

void lib_msconfig::slot_setDown()
{
    setItemRowDown();
}

//进入/退出编辑模式
void lib_msconfig::on_but_edit_clicked()
{
#if 0
    editMode ^= 1;
    ItemStruct addItem;
    int itemCount = itemList.size();
    for(int i = 0;i<itemCount;i++){
        addItem = itemList.at(i);
        if(ui->listWidget->currentItem() == addItem.item){
            break;
        }
    }
    if(ui->listWidget->currentItem()){
        if(editMode){//进入编辑模式
            ui->lineEdit->setText(ui->listWidget->currentItem()->text());
            ui->listWidget->setEnabled(false);
            ui->but_add->setEnabled(false);
            ui->but_save->setEnabled(false);
            ui->but_subtract->setEnabled(false);
        }else{//退出编辑模式
            QString editResult = ui->lineEdit->text();
            ui->lineEdit->setText("");
            ui->listWidget->currentItem()->setText(editResult);
            ui->listWidget->setEnabled(true);
            ui->but_add->setEnabled(true);
            ui->but_save->setEnabled(true);
            ui->but_subtract->setEnabled(true);
            if(itemCount > 0){
                if("#" == editResult.mid(0,1)){
                    addItem.box->setChecked(false);
                }else{
                    addItem.box->setChecked(true);
                }
            }
        }
    }else{
        editMode ^= 1;
    }
#endif

    if(openEditFlag){
        //直接将编辑内容替换到当前行
        ui->listWidget->currentItem()->setText(ui->lineEdit->text());
    }
}

void lib_msconfig::closeEdit()
{
//    qDebug()<<"lib_msconfig::closeEdit";
    openEditFlag = false;
    ui->lineEdit->setEnabled(false);
    ui->listWidget->setEnabled(false);
}


void lib_msconfig::openEdit()
{
//    qDebug()<<"lib_msconfig::openEdit";
    openEditFlag = true;
    ui->lineEdit->setEnabled(true);
    ui->listWidget->setEnabled(true);
}

void lib_msconfig::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current){
        ui->lineEdit->setText(current->text());
    }

    if(previous){

    }
}

void lib_msconfig::setIni()
{
    QFile startFile(filePath);
    if(startFile.open(QFile::ReadOnly)){
        QString startStr = startFile.readAll();
        startFile.close();
        QStringList startList = startStr.trimmed().split("\n");
        ui->listWidget->clear();
        itemList.clear();
        for(int i = 0;i<startList.size();i++){
            QString lineStr = startList.at(i);
            lineStr = lineStr.trimmed();
            if(lineStr.size() > 0)
            ui->listWidget->addItem(lineStr);
            myAddWidget();

        }
    }else{
        qDebug()<<"read open file:"<<startFile.errorString();
    }
}

void lib_msconfig::on_but_cancel_clicked()
{
    if(openEditFlag){
        setIni();
    }
}
