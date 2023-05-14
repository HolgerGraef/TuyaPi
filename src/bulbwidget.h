#ifndef BULB_WIDGET_H
#define BULB_WIDGET_H

#include <QWidget>

#include <nlohmann/json.hpp>
using ordered_json = nlohmann::ordered_json;

#include "iconbutton.h"

class BulbWidget : public QWidget
{
    Q_OBJECT

public:
    BulbWidget(const ordered_json& dev);

private:
    IconButton mBtnToggle;
};

#endif // BULB_WIDGET_H
