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
    BulbWidget(const ordered_json& dev, tuya::TuyaWorker& worker);

    void handleData(QString ip, QJsonDocument data);

public slots:
    void toggle();

private:
    tuya::TuyaWorker& mWorker;
    std::string mIp;

    IconButton mBtnToggle;
};

#endif // BULB_WIDGET_H
