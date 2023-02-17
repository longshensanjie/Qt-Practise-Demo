QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    confirmpage.cpp \
    main.cpp \
    mainwindow.cpp \
    opentreethread.cpp \
    picanimationwid.cpp \
    picbutton.cpp \
    picshow.cpp \
    picstatebtn.cpp \
    prelistitem.cpp \
    prelistwid.cpp \
    prosetpage.cpp \
    protree.cpp \
    protreeitem.cpp \
    protreethread.cpp \
    protreewidget.cpp \
    removeprodialog.cpp \
    slideshowdlg.cpp \
    wizard.cpp

HEADERS += \
    confirmpage.h \
    const.h \
    mainwindow.h \
    opentreethread.h \
    picanimationwid.h \
    picbutton.h \
    picshow.h \
    picstatebtn.h \
    prelistitem.h \
    prelistwid.h \
    prosetpage.h \
    protree.h \
    protreeitem.h \
    protreethread.h \
    protreewidget.h \
    removeprodialog.h \
    slideshowdlg.h \
    wizard.h

FORMS += \
    confirmpage.ui \
    mainwindow.ui \
    picshow.ui \
    prosetpage.ui \
    protree.ui \
    removeprodialog.ui \
    slideshowdlg.ui \
    wizard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

RC_ICONS = "icon/bulm.ico"
