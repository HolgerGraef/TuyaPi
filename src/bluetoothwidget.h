#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>

#include "bluetoothmanager.h"
#include "iconbutton.h"

class BluetoothWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BluetoothWidget(BluetoothManager* manager, QWidget *parent = nullptr);

private slots:
    void refresh();

private:
    BluetoothManager* mManager;
    QVBoxLayout mLayout;
    QMap<QString, IconButton*> mWidgets;
};

#endif // BLUETOOTHWIDGET_H
