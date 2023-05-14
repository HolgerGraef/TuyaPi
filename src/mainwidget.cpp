#include "mainwidget.h"

#include "bulbwidget.h"
#include "wifiwidget.h"

#include <QSpacerItem>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 1920, 1080);

    setStyleSheet("QWidget { font-size: 42pt; padding: 50px; }");

    QHBoxLayout *devLayout = new QHBoxLayout();
    const auto& devices = mTuyaWorker.scanner().knownDevices();
    for (const auto& dev : devices) {
        if (dev["category"] == "dj")  // light bulb
            devLayout->addWidget(new BulbWidget(dev));
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(devLayout);
    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new WifiWidget(&mWifiManager));
    setLayout(mainLayout);

    connect(&mTuyaWorker, SIGNAL(deviceDiscovered(QString)), this, SLOT(deviceDiscovered(QString)));
    connect(&mTuyaWorker, SIGNAL(newDeviceData(QString, QJsonDocument)), this, SLOT(newDeviceData(QString, QJsonDocument)));
    mTuyaWorker.start();
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
    std::cout << "Main widget received from " << ip.toStdString() << ": " << data.toJson().toStdString() << std::endl;
}
