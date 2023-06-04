#include "bulbwidget.h"

#include <QJsonObject>
#include <QVBoxLayout>

#include "main.h"

BrightnessOverlay::BrightnessOverlay(QWidget *parent, QString label) : Overlay(parent) {
    QVBoxLayout *layout = new QVBoxLayout();

    mCloseButton.setText(label);
    mCloseButton.setIcon(fa::mdi6_close);

    mSldBrightness.setMinimum(0);
    mSldBrightness.setMaximum(100);
    mSldBrightness.setOrientation(Qt::Orientation::Horizontal);
    mSldBrightness.setStyleSheet(
        "QSlider { "
            "height: 400px;"
            "background: transparent;"
        "}"
        "QSlider::groove:horizontal {"
            "height: 300px;"
            "background: #cccccc;"
            "margin: 2px 0;"
        "}"
        "QSlider::handle:horizontal {"
            "background: orange;"
            "width: 300px;"
            "margin: -2px 0;"
            "border-radius: 3px;"
        "}"
    );

    layout->addWidget(&mCloseButton);
    layout->addWidget(&mSldBrightness);

    setLayout(layout);

    connect(&mSldBrightness, SIGNAL(valueChanged(int)), this, SIGNAL(brightnessChanged(int)));
    connect(&mCloseButton, SIGNAL(clicked()), this, SLOT(hide()));
}

void BrightnessOverlay::setSliderValue(int value) {
    mSldBrightness.setValue(value);
}

BulbWidget::BulbWidget(QWidget *parent, std::shared_ptr<tuya::Device> dev) : QWidget(parent), mDev(dev), mBrightnessOverlay(parent, QString::fromStdString(dev->name())) {
    QVBoxLayout *layout = new QVBoxLayout();

    mNextBrightnessValue = 0;
    mDevIsBusy = false;
    mSetBrightnessTimer.setInterval(300);
    mSetBrightnessTimer.setSingleShot(true);
    connect(&mSetBrightnessTimer, SIGNAL(timeout()), this, SLOT(setNextBrightnessValue()));

    setEnabled(false);
    mBtnToggle.setText(QString::fromStdString(dev->name()));
    mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);

    mBtnBrightness.setText("Brightness");
    mBtnBrightness.setIcon(fa::mdi6_circle_slice_1);

    layout->addWidget(&mBtnToggle);
    layout->addWidget(&mBtnBrightness);

    setLayout(layout);

    connect(&mBtnToggle, SIGNAL(clicked()), this, SLOT(toggle()));
    connect(&mBtnBrightness, SIGNAL(clicked()), &mBrightnessOverlay, SLOT(show()));
    connect(&mBrightnessOverlay, SIGNAL(brightnessChanged(int)), this, SLOT(setBrightness(int)));
}

void BulbWidget::handleConnected(const QString &ip) {
    (void) ip;
    setEnabled(true);
    mBtnToggle.setIcon(mDev->isOn() ? fa::mdi6_lightbulb_on : fa::mdi6_lightbulb_off);
}

void BulbWidget::handleDisconnected(const QString &ip) {
    (void) ip;
    setEnabled(false);
    mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);
}

void BulbWidget::handleData(const QString& ip, const QJsonDocument& data) {
    (void) ip;
    if (data.isObject()) {
        if(data.object().contains("is_on")) {
            bool is_on = data.object()["is_on"].toBool();
            mBtnToggle.setIcon(is_on ? fa::mdi6_lightbulb_on : fa::mdi6_lightbulb_off);
        }
        if(data.object().contains("brightness")) {
            int brightness7 = data.object()["brightness"].toInt() * 7 / mDev->brightnessScale();
            mBtnBrightness.setIcon(fa::mdi6_circle_slice_1 + brightness7);
            mBrightnessOverlay.setSliderValue(data.object()["brightness"].toInt() * 100 / mDev->brightnessScale());
        }
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

/* set brightness on a scale of 0-100 */
void BulbWidget::setBrightness(int value) {
    if (mDevIsBusy) {
        mNextBrightnessValue = value;
        mSetBrightnessTimer.start();
        return;
    }

    mDevIsBusy = true;
    int ret = mDev->setBrightness(
        value * mDev->brightnessScale() / 100,
        [this, value](tuya::Device::CommandStatus status, const ordered_json& data) {
            (void) status;
            (void) data;
            mDevIsBusy = false;
            if (status != tuya::Device::CMD_OK) {
                mNextBrightnessValue = value;
                mSetBrightnessTimer.start();
            }
        }
    );
    if (ret != 0) {
        mNextBrightnessValue = value;
        mSetBrightnessTimer.start();
    }
}

void BulbWidget::setNextBrightnessValue() {
    setBrightness(mNextBrightnessValue);
}
