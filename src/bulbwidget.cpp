#include "bulbwidget.h"

#include <QJsonObject>
#include <QVBoxLayout>

#include "main.h"

BulbWidget::BulbWidget(std::shared_ptr<tuya::Device> dev) : mDev(dev) {
    QVBoxLayout *layout = new QVBoxLayout();

    mBtnToggle.setEnabled(false);
    mBtnToggle.setText(QString::fromStdString(dev->name()));
    mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);

    layout->addWidget(&mBtnToggle);

    setLayout(layout);

    connect(&mBtnToggle, SIGNAL(clicked()), this, SLOT(toggle()));
}

void BulbWidget::handleConnected(const QString &ip) {
    (void) ip;
    mBtnToggle.setEnabled(true);
    mBtnToggle.setIcon(mDev->isOn() ? fa::mdi6_lightbulb_on : fa::mdi6_lightbulb_off);
}

void BulbWidget::handleDisconnected(const QString &ip) {
    (void) ip;
    mBtnToggle.setEnabled(false);
    mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);
}

void BulbWidget::handleData(const QString& ip, const QJsonDocument& data) {
    (void) ip;
    if (data.isObject() && data.object().contains("is_on")) {
        bool is_on = data.object()["is_on"].toBool();
        mBtnToggle.setIcon(is_on ? fa::mdi6_lightbulb_on : fa::mdi6_lightbulb_off);
    }
}

void BulbWidget::toggle() {
    mBtnToggle.setEnabled(false);
    mDev->toggle([this](tuya::Device::CommandStatus status, const ordered_json&){
        if (status == tuya::Device::CMD_OK)
            mBtnToggle.setEnabled(true);
        else
            mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);
    });
}
