#include "lockscreen.h"

#include <QDateTime>

#include "version.h"

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

  mVersionLabel = new QLabel(mInfoOverlay);
  mVersionLabel->setText(getVersionString().c_str());
  mVersionLabel->setStyleSheet("QLabel { background: rgba(0,0,0,0%); color: "
                               "#CCCCCC; font-size: 20px; padding: 0px; }");
  mVersionLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);

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
    mVersionLabel->resize(parentWidget()->size());
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
