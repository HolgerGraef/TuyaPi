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

    mTuyaWorker.start();
}

MainWidget::~MainWidget()
{
    // TODO: gracefully terminate worker thread
}

