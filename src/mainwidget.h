#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "wifimanager.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    WifiManager mWifiManager;
};

#endif // MAINWIDGET_H
