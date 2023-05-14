QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

CONFIG += fontAwesomeFree
include(QtAwesome/QtAwesome/QtAwesome.pri)

CONFIG += tuyaCpp
include(tuyacpp/tuyacpp.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/iconbutton.cpp \
    src/main.cpp \
    src/mainwidget.cpp \
    src/wifimanager.cpp \
    src/wifiwidget.cpp

HEADERS += \
    src/iconbutton.h \
    src/main.h \
    src/mainwidget.h \
    src/wifimanager.h \
    src/wifiwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
