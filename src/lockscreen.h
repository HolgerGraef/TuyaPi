#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include "overlay.h"
#include "unlockoverlay.h"
#include "wifimanager.h"

class LockScreen : public Overlay
{
    Q_OBJECT
public:
    LockScreen(QWidget* parent, WifiManager& wifiManager);

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    void resetLockTimer();

public slots:
    void show();
    void refresh();

private:
    WifiManager& mWifiManager;

    QPixmap* mBackground;
    Overlay* mInfoOverlay;
    QTimer mLockTimer;
    QTimer mRefreshTimer;

    UnlockOverlay* mUnlockOverlay;
};

#endif // LOCKSCREEN_H
