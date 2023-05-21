#include "unlockoverlay.h"

#include <QDateTime>
#include <QPainter>

UnlockOverlay::UnlockOverlay(QWidget* parent) : Overlay(parent)
{
    mDiameter = 600;

    mTimer.setInterval(33);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(repaint()));
}

void UnlockOverlay::paintEvent(QPaintEvent *event)
{
    const auto& rect = QRectF((width() - mDiameter) / 2, (height() - mDiameter) / 2, mDiameter, mDiameter);
    QPainter painter(this);
    painter.setPen(QPen(QColor(0xcccccc), 200, Qt::SolidLine, Qt::FlatCap));
    painter.drawArc(rect, 0, progress() * 360 * 16 / 100);

    Overlay::paintEvent(event);
}

unsigned UnlockOverlay::progress()
{
    return 100 * (QDateTime::currentMSecsSinceEpoch() - mStartTimeMs) / mDurationMs;
}

void UnlockOverlay::start(unsigned int durationMs)
{
    mStartTimeMs = QDateTime::currentMSecsSinceEpoch();
    mDurationMs = durationMs;
    mTimer.start();
    show();
}
