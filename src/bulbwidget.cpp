#include "bulbwidget.h"

#include <QVBoxLayout>

BulbWidget::BulbWidget(const ordered_json& dev) {
    QVBoxLayout *layout = new QVBoxLayout();

    mBtnToggle.setText(QString::fromStdString(dev["name"]));

    layout->addWidget(&mBtnToggle);

    setLayout(layout);
}
