#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <tuyacpp/bindings/qt.hpp>

#include "wifimanager.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    WifiManager mWifiManager;
    tuya::TuyaWorker mTuyaWorker;
};

#endif // MAINWIDGET_H
