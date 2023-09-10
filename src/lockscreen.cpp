#include "lockscreen.h"

#include <QDateTime>

LockScreen::LockScreen(QWidget* parent,
                       BluetoothManager& bluetoothManager,
                       WifiManager& wifiManager)
  : Overlay(parent)
  , mBluetoothManager(bluetoothManager)
  , mWifiManager(wifiManager)
{
  mBackground = new QPixmap("background.png");

  mInfoOverlay = new Overlay(this);
  mInfoOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);

  mLockTimer.setInterval(5000);
  mLockTimer.setSingleShot(true);
  connect(&mLockTimer, SIGNAL(timeout()), this, SLOT(show()));
  mLockTimer.start();

  mRefreshTimer.setInterval(1000);
  connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
  mRefreshTimer.start();

  mUnlockOverlay = new UnlockOverlay(this);

  refresh();
}

void
LockScreen::mousePressEvent(QMouseEvent* event)
{
  (void)event;
  mUnlockOverlay->start(700);
}

void
LockScreen::mouseReleaseEvent(QMouseEvent* event)
{
  (void)event;
  if (mUnlockOverlay->isVisible()) {
    if (mUnlockOverlay->progress() >= 100) {
      hide();
      resetLockTimer();
    }
    mUnlockOverlay->hide();
  }
}

void
LockScreen::resetLockTimer()
{
  mLockTimer.start();
}

void
LockScreen::show()
{
  if (!isVisible()) {
    Overlay::show();
    mInfoOverlay->show();
    setPixmap(mBackground->scaled(size(), Qt::KeepAspectRatioByExpanding));
  }
}

void
LockScreen::refresh()
{
  const auto& dateTimeStr =
    QDateTime::currentDateTime().toString("dd MMM yyyy\nhh:mm\n") +
    mWifiManager.iconString() + mBluetoothManager.iconString();
  mInfoOverlay->setText(dateTimeStr);
}
