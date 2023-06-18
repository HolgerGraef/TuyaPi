#ifndef UNLOCKOVERLAY_H
#define UNLOCKOVERLAY_H

#include <QTimer>

#include "overlay.h"

class UnlockOverlay : public Overlay
{
  Q_OBJECT
public:
  UnlockOverlay(QWidget* parent);

  virtual void paintEvent(QPaintEvent* event) override;
  unsigned progress();
  void start(unsigned durationMs);

private:
  QTimer mTimer;

  qint64 mStartTimeMs;
  unsigned mDurationMs;
  unsigned mDiameter;
};

#endif // UNLOCKOVERLAY_H
