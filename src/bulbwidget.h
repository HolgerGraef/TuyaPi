#ifndef BULB_WIDGET_H
#define BULB_WIDGET_H

#include <QJsonDocument>
#include <QSlider>
#include <QTimer>
#include <QWidget>
#include <nlohmann/json.hpp>
using ordered_json = nlohmann::ordered_json;

#include <tuyacpp/bindings/qt.hpp>

#include "iconbutton.h"
#include "overlay.h"

class BulbWidget;

class SliderOverlay : public Overlay
{
  Q_OBJECT

public:
  SliderOverlay(QWidget* parent, QString label);

  void setSliderValue(int value);

signals:
  void valueChanged(int);
  void sliderPressed();
  void sliderReleased();

private:
  IconButton mCloseButton;
  QSlider mSlider;
};

class BulbWidget : public QWidget
{
  Q_OBJECT

public:
  BulbWidget(QWidget* parent, std::shared_ptr<tuya::Device> dev);

  void handleConnected(const QString& ip);
  void handleDisconnected(const QString& ip);
  void handleData(const QString& ip, const QJsonDocument& data);

public slots:
  void toggle();
  void setBrightness(int value);
  void setColorTemp(int value);
  void setNextBrightnessValue();
  void setNextColortempValue();
  void sliderPressed();
  void sliderReleased();

private:
  std::shared_ptr<tuya::Device> mDev;

  IconButton mBtnToggle;
  IconButton mBtnBrightness;
  std::unique_ptr<SliderOverlay> mBrightnessOverlay;
  IconButton mBtnColorTemp;
  std::unique_ptr<SliderOverlay> mColorTempOverlay;

  bool mDevIsBusy;
  std::atomic_bool mSliderIsActive;
  int mNextBrightnessValue;
  QTimer mSetBrightnessTimer;
  int mNextColorTempValue;
  QTimer mSetColorTempTimer;
};

#endif // BULB_WIDGET_H
