QT       += core bluetooth gui

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
    src/bluetoothmanager.cpp \
    src/bluetoothwidget.cpp \
    src/iconbutton.cpp \
    src/lockscreen.cpp \
    src/main.cpp \
    src/mainwidget.cpp \
    src/overlay.cpp \
    src/unlockoverlay.cpp \
    src/wifimanager.cpp \
    src/wifiwidget.cpp \
    src/bulbwidget.cpp

HEADERS += \
    src/bluetoothmanager.h \
    src/bluetoothwidget.h \
    src/iconbutton.h \
    src/lockscreen.h \
    src/main.h \
    src/mainwidget.h \
    src/overlay.h \
    src/unlockoverlay.h \
    src/version.h \
    src/wifimanager.h \
    src/wifiwidget.h \
    src/bulbwidget.h

linux-g++ {
!build_pass:message(building for desktop)
DEFINES += TUYAPI_IS_DESKTOP
QMAKE_CXXFLAGS += -DTUYAPI_IS_DESKTOP
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
