#include "bulbwidget.h"

#include <QJsonObject>
#include <QVBoxLayout>

#include "main.h"

BulbWidget::BulbWidget(const ordered_json& dev, tuya::TuyaWorker& worker) : mWorker(worker), mIp(dev["ip"]) {
    QVBoxLayout *layout = new QVBoxLayout();

    mBtnToggle.setText(QString::fromStdString(dev["name"]));
    mBtnToggle.setIcon(awesome()->icon(fa::mdi6, fa::mdi6_lightbulb_outline));

    layout->addWidget(&mBtnToggle);

    setLayout(layout);

    connect(&mBtnToggle, SIGNAL(clicked()), this, SLOT(toggle()));
}

void BulbWidget::handleData(QString ip, QJsonDocument data) {
    (void) ip;
    if (data.isObject() && data.object().contains("is_on")) {
        bool is_on = data.object()["is_on"].toBool();
        mBtnToggle.setIcon(awesome()->icon(fa::mdi6, is_on ? fa::mdi6_lightbulb_on : fa::mdi6_lightbulb_off));
    }
}

void BulbWidget::toggle() {
    auto dev = mWorker.scanner().getDevice(mIp);
    if (dev)
        dev->setOn(true);
}
