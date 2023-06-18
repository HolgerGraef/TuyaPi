#ifndef WIFIWIDGET_H
#define WIFIWIDGET_H

#include <QWidget>

#include "iconbutton.h"
#include "wifimanager.h"

class WifiWidget : public IconButton
{
    Q_OBJECT
public:
    explicit WifiWidget(WifiManager* manager, QWidget *parent = nullptr);

private slots:
    void refresh();

private:
    WifiManager* mManager;
};

#endif // WIFIWIDGET_H
