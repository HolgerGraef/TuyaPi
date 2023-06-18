#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QLabel>

#include <tuyacpp/bindings/qt.hpp>

#include "bluetoothmanager.h"
#include "bulbwidget.h"
#include "lockscreen.h"
#include "wifimanager.h"

class MainWidget : public QLabel
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void deviceConnected(QString ip);
    void deviceDisconnected(QString ip);
    void deviceDiscovered(QString ip);
    void newDeviceData(QString ip, QJsonDocument data);

private:
    LockScreen* mLockScreen;
    std::map<QString, BulbWidget*> mBulbWidgets;
    BluetoothManager mBluetoothManager;
    WifiManager mWifiManager;
    tuya::TuyaWorker mTuyaWorker;
};

#endif // MAINWIDGET_H
