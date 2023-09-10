#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include "bluetoothmanager.h"
#include "overlay.h"
#include "unlockoverlay.h"
#include "wifimanager.h"

class LockScreen : public Overlay
{
  Q_OBJECT
public:
  LockScreen(QWidget* parent,
             BluetoothManager& bluetoothManager,
             WifiManager& wifiManager);

  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  void resetLockTimer();

public slots:
  void show();
  void refresh();

private:
  BluetoothManager& mBluetoothManager;
  WifiManager& mWifiManager;

  QPixmap* mBackground;
  Overlay* mInfoOverlay;
  QLabel* mVersionLabel;
  QTimer mLockTimer;
  QTimer mRefreshTimer;

  UnlockOverlay* mUnlockOverlay;
};

#endif // LOCKSCREEN_H
