#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QTimer>
#include <QIcon>

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>

class BluetoothManager : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothManager(QObject *parent = nullptr);
    QStringList devices() const;
    const QIcon icon() const;
    QString iconString() const;

signals:
    void updated();

private slots:
    void refresh();

    void onDeviceDiscovered(const QBluetoothDeviceInfo&);

private:
    QBluetoothDeviceDiscoveryAgent mAgent;
    QBluetoothLocalDevice mLocalDevice;

    QList<QBluetoothDeviceInfo> mDevices;

    QTimer mRefreshTimer;
};

#endif // BLUETOOTHMANAGER_H
