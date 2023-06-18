#include "overlay.h"

Overlay::Overlay(QWidget* parent)
  : QLabel(parent)
{
  setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  setStyleSheet("background: rgba(0,0,0,70%); color: #CCCCCC; font-size: "
                "200px; padding: 0px;");

  hide();
}

void
Overlay::show()
{
  resize(parentWidget()->size());
  QLabel::show();
  raise();
}
