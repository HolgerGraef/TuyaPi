#include "mainwidget.h"

#include <QApplication>
#include <QBitmap>
#include <QEvent>
#include <QSpacerItem>
#include <QVBoxLayout>

#include "bluetoothwidget.h"
#include "wifiwidget.h"

#include "main.h"

MainWidget::MainWidget(bool isDesktop, QWidget* parent)
  : QLabel(parent)
{
  auto iconOptions = QVariantMap{ { "color", QColor(Qt::lightGray) } };

  if (!isDesktop) {
    setGeometry(0, 0, 1920, 1080);
    setStyleSheet("QWidget { font-size: 42pt; }");

    setPixmap(
      QPixmap("background.png").scaled(size(), Qt::KeepAspectRatioByExpanding));

    auto icon = awesome()->icon(fa::mdi6, fa::mdi6_circle_slice_8, iconOptions);
    auto pm = icon.pixmap(150, 150);
    mCursor = QCursor(pm, -1, -1);
    setCursor(mCursor);

    connect(&mMouseHideTimer, SIGNAL(timeout()), this, SLOT(hideMouse()));
    mMouseHideTimer.setInterval(1000);
    mMouseHideTimer.start();

    mLockScreen = new LockScreen(this, mBluetoothManager, mWifiManager);
  } else {
    setGeometry(0, 0, 800, 600);
    auto icon = awesome()->icon(fa::mdi6, fa::mdi6_home, iconOptions);

    mQuitAction = new QAction(tr("&Quit"), this);
    connect(mQuitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    mTrayIconMenu = new QMenu(this);
    mTrayIconMenu->addSeparator();
    mTrayIconMenu->addAction(mQuitAction);

    mTrayIcon = new QSystemTrayIcon(this);
    mTrayIcon->setContextMenu(mTrayIconMenu);

    mTrayIcon->setIcon(icon);
    setWindowIcon(icon);

    mTrayIcon->show();
  }

  QHBoxLayout* devLayout = new QHBoxLayout();
  const auto& devices = mTuyaWorker.scanner().knownDevices();
  for (const auto& dev : devices) {
    if (dev["category"] == "dj") { // light bulb
      auto bulb =
        new BulbWidget(this, mTuyaWorker.scanner().getDevice(dev["ip"]));
      mBulbWidgets[QString::fromStdString(dev["ip"])] = bulb;
      devLayout->addWidget(bulb);
    }
  }

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addLayout(devLayout);
  if (!isDesktop) {
    mainLayout->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new BluetoothWidget(&mBluetoothManager));
    mainLayout->addWidget(new WifiWidget(&mWifiManager));
  }
  setLayout(mainLayout);

  connect(&mTuyaWorker,
          SIGNAL(deviceDiscovered(QString)),
          this,
          SLOT(deviceDiscovered(QString)));
  // reduce overhead
  connect(&mTuyaWorker,
          SIGNAL(newDeviceData(QString, QJsonDocument)),
          this,
          SLOT(newDeviceData(QString, QJsonDocument)));
  connect(&mTuyaWorker,
          SIGNAL(deviceConnected(QString)),
          this,
          SLOT(deviceConnected(QString)));
  connect(&mTuyaWorker,
          SIGNAL(deviceDisconnected(QString)),
          this,
          SLOT(deviceDisconnected(QString)));
  mTuyaWorker.start();

  if (!isDesktop) {
    installEventFilter(this);
    auto childrenList = children();
    while (childrenList.length()) {
      auto c = childrenList.takeFirst();
      c->installEventFilter(this);
      childrenList += c->children();
    }
  }
}

bool
MainWidget::eventFilter(QObject* watched, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress) {
    setCursor(mCursor);
    mMouseHideTimer.stop();
    mLockScreen->resetLockTimer();
    return false;
  } else if (event->type() == QEvent::MouseMove) {
    mLockScreen->resetLockTimer();
    setCursor(mCursor);
    mMouseHideTimer.start();
    return false;
  } else if (event->type() == QEvent::MouseButtonRelease) {
    mLockScreen->resetLockTimer();
    setCursor(mCursor);
    mMouseHideTimer.start();
    return false;
  } else {
    return QLabel::eventFilter(watched, event);
  }
}

MainWidget::~MainWidget()
{
  for (auto& bulb : mBulbWidgets)
    delete bulb.second;
  mBulbWidgets.clear();

  mTuyaWorker.stop();
  mTuyaWorker.wait();
}

void
MainWidget::deviceConnected(QString ip)
{
  auto bulb = mBulbWidgets.find(ip);
  if (bulb != mBulbWidgets.end())
    bulb->second->handleConnected(ip);
}

void
MainWidget::deviceDisconnected(QString ip)
{
  auto bulb = mBulbWidgets.find(ip);
  if (bulb != mBulbWidgets.end())
    bulb->second->handleDisconnected(ip);
}

void
MainWidget::deviceDiscovered(QString ip)
{
  (void)ip;
}

void
MainWidget::newDeviceData(QString ip, QJsonDocument data)
{
  auto bulb = mBulbWidgets.find(ip);
  if (bulb != mBulbWidgets.end())
    bulb->second->handleData(ip, data);
}

void
MainWidget::hideMouse()
{
  mMouseHideTimer.stop();
  setCursor(Qt::BlankCursor);
}
