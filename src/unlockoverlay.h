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
  void start(unsigned durationMs = DEFAULT_DURATION_MS);

  static const unsigned int DEFAULT_DURATION_MS = 700;
  static const unsigned int DEFAULT_DIAMETER = 600;

private:
  QTimer mTimer;

  qint64 mStartTimeMs;
  unsigned mDurationMs;
  unsigned mDiameter;
};

#endif // UNLOCKOVERLAY_H
