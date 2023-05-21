#include "mainwidget.h"

#include "wifiwidget.h"

#include <QEvent>
#include <QSpacerItem>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
    : QLabel(parent)
{
    setGeometry(0, 0, 1920, 1080);
    setStyleSheet("QWidget { font-size: 42pt; }");

    setPixmap(QPixmap("background.png").scaled(size(), Qt::KeepAspectRatioByExpanding));

    mLockScreen = new LockScreen(this, mWifiManager);

    QHBoxLayout *devLayout = new QHBoxLayout();
    const auto& devices = mTuyaWorker.scanner().knownDevices();
    for (const auto& dev : devices) {
        if (dev["category"] == "dj") { // light bulb
            auto bulb = new BulbWidget(mTuyaWorker.scanner().getDevice(dev["ip"]));
            mBulbWidgets[QString::fromStdString(dev["ip"])] = bulb;
            devLayout->addWidget(bulb);
        }
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(devLayout);
    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new WifiWidget(&mWifiManager));
    setLayout(mainLayout);

    connect(&mTuyaWorker, SIGNAL(deviceDiscovered(QString)), this, SLOT(deviceDiscovered(QString)));
    connect(&mTuyaWorker, SIGNAL(newDeviceData(QString, QJsonDocument)), this, SLOT(newDeviceData(QString, QJsonDocument)));
    mTuyaWorker.start();

    installEventFilter(this);
    auto childrenList = children();
    while (childrenList.length()) {
        auto c = childrenList.takeFirst();
        c->installEventFilter(this);
        childrenList += c->children();
    }
}

bool MainWidget::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        mLockScreen->resetLockTimer();
        return false;
    } else {
        return QLabel::eventFilter(watched, event);
    }

}

MainWidget::~MainWidget()
{
    // TODO: gracefully terminate worker thread
}

void MainWidget::deviceDiscovered(QString ip)
{
    (void) ip;
}

void MainWidget::newDeviceData(QString ip, QJsonDocument data)
{
    auto bulb = mBulbWidgets.find(ip);
    if (bulb != mBulbWidgets.end())
        bulb->second->handleData(ip, data);
}
