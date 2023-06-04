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

class BrightnessOverlay : public Overlay
{
    Q_OBJECT

public:
    BrightnessOverlay(QWidget *parent, QString label);
    void setSliderValue(int value);

signals:
    void brightnessChanged(int);

private:
    IconButton mCloseButton;
    QSlider mSldBrightness;
};

class BulbWidget : public QWidget
{
    Q_OBJECT

public:
    BulbWidget(QWidget *parent, std::shared_ptr<tuya::Device> dev);

    void handleConnected(const QString& ip);
    void handleDisconnected(const QString& ip);
    void handleData(const QString& ip, const QJsonDocument& data);

public slots:
    void toggle();
    void setBrightness(int value);
    void setNextBrightnessValue();

private:
    std::shared_ptr<tuya::Device> mDev;

    IconButton mBtnToggle;
    IconButton mBtnBrightness;
    BrightnessOverlay mBrightnessOverlay;

    bool mDevIsBusy;
    int mNextBrightnessValue;
    QTimer mSetBrightnessTimer;
};

#endif // BULB_WIDGET_H
