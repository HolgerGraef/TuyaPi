#include "bulbwidget.h"

#include <QJsonObject>
#include <QVBoxLayout>

#include "main.h"

SliderOverlay::SliderOverlay(QWidget* parent, QString label)
  : Overlay(parent)
{
  QVBoxLayout* layout = new QVBoxLayout();

  mCloseButton.setText(label);
  mCloseButton.setIcon(fa::mdi6_close);

  mSlider.setMinimum(0);
  mSlider.setMaximum(100);
  mSlider.setOrientation(Qt::Orientation::Horizontal);
  mSlider.setStyleSheet("QSlider { "
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
                        "}");

  layout->addWidget(&mCloseButton);
  layout->addWidget(&mSlider);

  setLayout(layout);

  connect(&mSlider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
  connect(&mSlider, SIGNAL(sliderPressed()), this, SIGNAL(sliderPressed()));
  connect(&mSlider, SIGNAL(sliderReleased()), this, SIGNAL(sliderReleased()));
  connect(&mCloseButton, SIGNAL(clicked()), this, SLOT(hide()));
}

void
SliderOverlay::setSliderValue(int value)
{
  mSlider.setValue(value);
}

BulbWidget::BulbWidget(QWidget* parent, std::shared_ptr<tuya::Device> dev)
  : QWidget(parent)
  , mDev(dev)
  , mBrightnessOverlay(
      std::make_unique<SliderOverlay>(parent,
                                      QString::fromStdString(dev->name())))
  , mColorTempOverlay(
      std::make_unique<SliderOverlay>(parent,
                                      QString::fromStdString(dev->name())))
  , mSliderIsActive(false)
{
  QVBoxLayout* layout = new QVBoxLayout();

  mNextBrightnessValue = 0;
  mDevIsBusy = false;
  mSetBrightnessTimer.setInterval(300);
  mSetBrightnessTimer.setSingleShot(true);
  connect(&mSetBrightnessTimer,
          SIGNAL(timeout()),
          this,
          SLOT(setNextBrightnessValue()));

  mNextColorTempValue = 0;
  mSetColorTempTimer.setInterval(300);
  mSetColorTempTimer.setSingleShot(true);
  connect(&mSetColorTempTimer,
          SIGNAL(timeout()),
          this,
          SLOT(setNextColortempValue()));

  setEnabled(false);
  mBtnToggle.setText(QString::fromStdString(dev->name()));
  mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);

  mBtnBrightness.setText("Brightness");
  mBtnBrightness.setIcon(fa::mdi6_circle_slice_1);

  mBtnColorTemp.setText("Color temperature");
  mBtnColorTemp.setIcon(fa::mdi6_circle_slice_1);

  layout->addWidget(&mBtnToggle);
  layout->addWidget(&mBtnBrightness);
  layout->addWidget(&mBtnColorTemp);

  setLayout(layout);

  connect(&mBtnToggle, SIGNAL(clicked()), this, SLOT(toggle()));
  connect(
    &mBtnBrightness, SIGNAL(clicked()), mBrightnessOverlay.get(), SLOT(show()));
  connect(mBrightnessOverlay.get(),
          SIGNAL(valueChanged(int)),
          this,
          SLOT(setBrightness(int)));
  connect(mBrightnessOverlay.get(),
          SIGNAL(sliderPressed()),
          this,
          SLOT(sliderPressed()));

  connect(mBrightnessOverlay.get(),
          SIGNAL(sliderReleased()),
          this,
          SLOT(sliderReleased()));

  connect(
    &mBtnColorTemp, SIGNAL(clicked()), mColorTempOverlay.get(), SLOT(show()));
  connect(mColorTempOverlay.get(),
          SIGNAL(valueChanged(int)),
          this,
          SLOT(setColorTemp(int)));
  connect(mColorTempOverlay.get(),
          SIGNAL(sliderPressed()),
          this,
          SLOT(sliderPressed()));

  connect(mColorTempOverlay.get(),
          SIGNAL(sliderReleased()),
          this,
          SLOT(sliderReleased()));
}

void
BulbWidget::handleConnected(const QString& ip)
{
  (void)ip;
  setEnabled(true);
  mBtnToggle.setIcon(mDev->isOn() ? fa::mdi6_lightbulb_on
                                  : fa::mdi6_lightbulb_off);
}

void
BulbWidget::handleDisconnected(const QString& ip)
{
  (void)ip;
  setEnabled(false);
  mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);
}

void
BulbWidget::handleData(const QString& ip, const QJsonDocument& data)
{
  (void)ip;
  if (data.isObject()) {
    if (data.object().contains("is_on")) {
      bool is_on = data.object()["is_on"].toBool();
      mBtnToggle.setIcon(is_on ? fa::mdi6_lightbulb_on
                               : fa::mdi6_lightbulb_off);
    }
    if (data.object().contains("brightness")) {
      int brightness7 =
        data.object()["brightness"].toInt() * 7 / mDev->brightnessScale();
      mBtnBrightness.setIcon(fa::mdi6_circle_slice_1 + brightness7);
      if (!mSliderIsActive)
        mBrightnessOverlay->setSliderValue(data.object()["brightness"].toInt() *
                                           100 / mDev->brightnessScale());
    }
    if (data.object().contains("colourtemp")) {
      int colortemp7 =
        data.object()["colourtemp"].toInt() * 7 / mDev->colorTempScale();
      mBtnColorTemp.setIcon(fa::mdi6_circle_slice_1 + colortemp7);
      if (!mSliderIsActive)
        mColorTempOverlay->setSliderValue(data.object()["colourtemp"].toInt() *
                                          100 / mDev->colorTempScale());
    }
  }
}

void
BulbWidget::toggle()
{
  mBtnToggle.setEnabled(false);
  mBtnToggle.setSpinner();
  mDev->toggle([this](tuya::Device::CommandStatus status, const ordered_json&) {
    if (status == tuya::Device::CMD_OK)
      mBtnToggle.setEnabled(true);
    else
      mBtnToggle.setIcon(fa::mdi6_lightbulb_outline);
  });
}

/* set brightness on a scale of 0-100 */
void
BulbWidget::setBrightness(int value)
{
  if (mDevIsBusy) {
    mNextBrightnessValue = value;
    mSetBrightnessTimer.start();
    return;
  }

  mDevIsBusy = true;
  mNextBrightnessValue = value;
  int ret =
    mDev->setBrightness(value * mDev->brightnessScale() / 100,
                        [this, value](tuya::Device::CommandStatus status,
                                      const ordered_json& data) {
                          (void)status;
                          (void)data;
                          mDevIsBusy = false;
                          if (status != tuya::Device::CMD_OK)
                            mSetBrightnessTimer.start();
                        });
  if (ret != 0)
    mSetBrightnessTimer.start();
}

void
BulbWidget::sliderPressed()
{
  mSliderIsActive = true;
}

void
BulbWidget::sliderReleased()
{
  mSliderIsActive = false;
}

void
BulbWidget::setNextBrightnessValue()
{
  setBrightness(mNextBrightnessValue);
}

void
BulbWidget::setColorTemp(int value)
{
  if (mDevIsBusy) {
    mNextColorTempValue = value;
    mSetColorTempTimer.start();
    return;
  }

  mDevIsBusy = true;
  mNextColorTempValue = value;
  int ret = mDev->setColorTemp(value * mDev->colorTempScale() / 100,
                               [this, value](tuya::Device::CommandStatus status,
                                             const ordered_json& data) {
                                 (void)status;
                                 (void)data;
                                 mDevIsBusy = false;
                                 if (status != tuya::Device::CMD_OK)
                                   mSetColorTempTimer.start();
                               });
  if (ret != 0)
    mSetColorTempTimer.start();
}

void
BulbWidget::setNextColortempValue()
{
  setColorTemp(mNextColorTempValue);
}
