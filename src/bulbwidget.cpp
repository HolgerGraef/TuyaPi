#include "bulbwidget.h"

#include <QJsonObject>
#include <QVBoxLayout>

BulbWidget::BulbWidget(const ordered_json& dev) {
    QVBoxLayout *layout = new QVBoxLayout();

    mBtnToggle.setText(QString::fromStdString(dev["name"]));

    layout->addWidget(&mBtnToggle);

    setLayout(layout);
}

void BulbWidget::handleData(QString ip, QJsonDocument data) {
    (void) ip;
    if (data.isObject() && data.object().contains("is_on")) {
        bool is_on = data.object()["is_on"].toBool();
        mBtnToggle.setText(is_on ? "on" : "off");
    }
}
