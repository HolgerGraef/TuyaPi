#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QLabel>

#include <tuyacpp/bindings/qt.hpp>

#include "bulbwidget.h"
#include "lockscreen.h"
#include "wifimanager.h"

class MainWidget : public QLabel
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void deviceDiscovered(QString ip);
    void newDeviceData(QString ip, QJsonDocument data);

private:
    LockScreen* mLockScreen;
    std::map<QString, BulbWidget*> mBulbWidgets;
    WifiManager mWifiManager;
    tuya::TuyaWorker mTuyaWorker;
};

#endif // MAINWIDGET_H
