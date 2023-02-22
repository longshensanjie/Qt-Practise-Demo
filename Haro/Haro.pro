QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dresswindow.cpp \
    main.cpp \
    haro.cpp \
    musicwindow.cpp \
    setwindow.cpp

HEADERS += \
    const.h \
    dresswindow.h \
    haro.h \
    musicwindow.h \
    setwindow.h

FORMS += \
    dresswindow.ui \
    haro.ui \
    musicwindow.ui \
    setwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
