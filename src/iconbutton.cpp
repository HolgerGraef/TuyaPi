#include "iconbutton.h"

#include <QHBoxLayout>

IconButton::IconButton(QWidget *parent)
    : QPushButton{parent}
{
    setIconSize(QSize(64, 64));
    setStyleSheet("text-align: left;");

    mLabel.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    mLabel.setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(&mLabel);
    setLayout(layout);
}

void IconButton::setText(const QString &text)
{
    mLabel.setText(text);
}
