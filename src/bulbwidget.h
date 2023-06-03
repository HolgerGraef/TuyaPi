#ifndef BULB_WIDGET_H
#define BULB_WIDGET_H

#include <QJsonDocument>
#include <QWidget>

#include <nlohmann/json.hpp>
using ordered_json = nlohmann::ordered_json;

#include <tuyacpp/bindings/qt.hpp>

#include "iconbutton.h"

class BulbWidget : public QWidget
{
    Q_OBJECT

public:
    BulbWidget(std::shared_ptr<tuya::Device> dev);

    void handleConnected(const QString& ip);
    void handleDisconnected(const QString& ip);
    void handleData(const QString& ip, const QJsonDocument& data);

public slots:
    void toggle();

private:
    std::shared_ptr<tuya::Device> mDev;

    IconButton mBtnToggle;
};

#endif // BULB_WIDGET_H
