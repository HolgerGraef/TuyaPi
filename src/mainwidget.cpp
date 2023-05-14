#include "mainwidget.h"

#include "wifiwidget.h"

#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 1920, 1080);

    setStyleSheet("QWidget { font-size: 42pt; padding: 50px; }");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(new WifiWidget(&mWifiManager));
    setLayout(mainLayout);

    mTuyaWorker.start();
}

MainWidget::~MainWidget()
{
    // TODO: gracefully terminate worker thread
}

