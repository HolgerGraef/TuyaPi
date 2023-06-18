#include "bluetoothmanager.h"

#include "main.h"

#include <iostream>

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject{parent}
{
    connect(&mAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)), this, SLOT(onDeviceDiscovered(const QBluetoothDeviceInfo&)));
    connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));

    mRefreshTimer.setInterval(500);
    mRefreshTimer.start();

    refresh();
}

QStringList BluetoothManager::devices() const
{
    QStringList result;
    for (const auto& dev : mDevices)
        result.append(dev.name());
    return result;
}

const QIcon BluetoothManager::icon() const
{
    const auto &bt_con = fa::mdi6_bluetooth_connect;
    const auto &bt_notcon = fa::mdi6_bluetooth_off;
    return awesome()->icon(fa::mdi6, mDevices.size() > 0 ? bt_con : bt_notcon);
}

QString BluetoothManager::iconString() const
{
    const auto &bt_con = fa::mdi6_bluetooth_connect;
    const auto &bt_notcon = fa::mdi6_bluetooth_off;
    return QString::fromStdU32String(std::u32string({static_cast<char32_t>(mDevices.size() > 0 ? bt_con : bt_notcon)}));
}

void BluetoothManager::onDeviceDiscovered(const QBluetoothDeviceInfo& dev)
{
    if (mLocalDevice.connectedDevices().contains(dev.address()))
        mDevices.append(dev);
}

void BluetoothManager::refresh()
{
    if (!mAgent.isActive()) {
        mDevices.clear();
        mAgent.start();
    } else {
        mAgent.stop();
        emit updated();
    }
}
