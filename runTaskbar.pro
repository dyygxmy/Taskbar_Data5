#-------------------------------------------------
#
# Project created by QtCreator 2019-11-26T10:20:44
#
#-------------------------------------------------

QT       += core gui serialport core-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taskbar_21.27.1
TEMPLATE = app

DESTDIR += ./
LIBS += -ludev -lSUSI-4.00


SOURCES += main.cpp\
        mainwindow.cpp \
    lib_taskbar_wi.cpp \
    connectcp2110.cpp \
    connectcp2112.cpp \
    connectyanhuaio.cpp \
    hotspotmanage.cpp \
    iwconfigmsg.cpp \
    lib_connectiobox.cpp \
    lib_datetimemanage.cpp \
    lib_iomanage.cpp \
    lib_msconfig.cpp \
    lib_networkmanage.cpp \
    lib_reoff.cpp \
    lib_setnetwork.cpp \
    mycombobox.cpp \
    mymessagebox.cpp \
    mysleep.cpp \
    ntpmanage.cpp \
    scanwlan.cpp \
    gpio_manage/cp2112_hidapi.cpp \
    gpio_manage/myhidapi.c \
    DYKeyboard/dialogtooltip.cpp \
    DYKeyboard/formhanziselector.cpp \
    DYKeyboard/formhanzitable.cpp \
    DYKeyboard/formnumpad.cpp \
    DYKeyboard/formsymbolpad.cpp \
    DYKeyboard/HanziManager.cpp \
    DYKeyboard/keyboard.cpp \
    DYKeyboard/myqapplication.cpp \
    DYKeyboard/padbase.cpp

HEADERS  += mainwindow.h \
    lib_taskbar_wi.h \
    connectcp2110.h \
    connectcp2112.h \
    connectyanhuaio.h \
    hotspotmanage.h \
    iwconfigmsg.h \
    lib_connectiobox.h \
    lib_datetimemanage.h \
    lib_iomanage.h \
    lib_msconfig.h \
    lib_networkmanage.h \
    lib_reoff.h \
    lib_setnetwork.h \
    lib_taskbar_global.h \
    mycombobox.h \
    mymessagebox.h \
    mysleep.h \
    ntpmanage.h \
    scanwlan.h \
    gpio_manage/cp2112_hidapi.h \
    gpio_manage/myhidapi.h \
    gpio_manage/Susi4.h \
    DYKeyboard/ChineseTranslator.h \
    DYKeyboard/dialogtooltip.h \
    DYKeyboard/formhanziselector.h \
    DYKeyboard/formhanzitable.h \
    DYKeyboard/formnumpad.h \
    DYKeyboard/formsymbolpad.h \
    DYKeyboard/HanziManager.h \
    DYKeyboard/keyboard.h \
    DYKeyboard/myqapplication.h \
    DYKeyboard/padbase.h

FORMS    += mainwindow.ui \
    lib_taskbar_wi.ui \
    lib_datetimemanage.ui \
    lib_iomanage.ui \
    lib_msconfig.ui \
    lib_networkmanage.ui \
    lib_reoff.ui \
    lib_setnetwork.ui \
    mymessagebox.ui \
    DYKeyboard/dialogtooltip.ui \
    DYKeyboard/formhanziselector.ui \
    DYKeyboard/formhanzitable.ui \
    DYKeyboard/formnumpad.ui \
    DYKeyboard/formsymbolpad.ui \
    DYKeyboard/keyboard.ui

RESOURCES += \
    Icon/icon.qrc \
    DYKeyboard/keypad.qrc \
    icon2.qrc

DISTFILES += \
    runTaskbar.pro.user \
    runTaskbar.pro.user.1fea126 \
    runTaskbar.pro.user.a63b2f6 \
    runTaskbar.pro.user.fc73324 \
    gpio_manage/gpio_manage.zip \
    LCD_CS351/35_all/bk-color_9.bmp \
    DYKeyboard/pinyin
