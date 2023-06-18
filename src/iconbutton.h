#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <QtAwesome.h>

class IconButton : public QPushButton
{
  Q_OBJECT
public:
  explicit IconButton(QWidget* parent = nullptr);
  void setIcon(const QIcon& icon);
  void setIcon(int charCode);
  void setIcon(const QString& iconString);
  void setText(const QString& text);

  void setSpinner();

private:
  QLabel mLabel;
  fa::QtAwesomeAnimation mAnimation;

signals:
};

#endif // ICONBUTTON_H
