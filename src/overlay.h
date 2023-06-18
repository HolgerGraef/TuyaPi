#ifndef OVERLAY_H
#define OVERLAY_H

#include <QLabel>

class Overlay : public QLabel
{
  Q_OBJECT
public:
  Overlay(QWidget* parent);

public slots:
  void show();
};

#endif // OVERLAY_H
