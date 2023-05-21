#include "wifiwidget.h"

#include "main.h"

WifiWidget::WifiWidget(WifiManager *manager, QWidget *parent)
    : IconButton{parent}, mManager(manager)
{  
    connect(mManager, SIGNAL(updated()), this, SLOT(refresh()));
    refresh();
}

void WifiWidget::refresh()
{
    if (mManager->essid().length())
        setText(mManager->essid() + "  |  " + QString::number(mManager->signal()) + " dBm");
    else
        setText("Not connected");

    setIcon(mManager->iconString());
}
