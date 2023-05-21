#include "iconbutton.h"

#include <QHBoxLayout>

IconButton::IconButton(QWidget *parent)
    : QPushButton{parent}
{
    setStyleSheet("IconButton { border: 0px; padding: 50px; text-align: left; background: rgba(0,0,0,70%); color: rgba(220, 220, 220, 100%); } IconButton:!enabled { background: rgba(0,0,0,30%); color: rgba(122, 122, 122, 100%); }");

    mLabel.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    mLabel.setStyleSheet("QLabel { background: transparent; color: rgba(220, 220, 220, 100%); } QLabel:!enabled { color: rgba(122, 122, 122, 100%); }");
    mLabel.setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(&mLabel);
    setLayout(layout);
}

void IconButton::setIcon(int charCode)
{
    QPushButton::setText(QString::fromStdU32String(std::u32string({static_cast<char32_t>(charCode)})));
}

void IconButton::setIcon(const QString& iconString)
{
    QPushButton::setText(iconString);
}

void IconButton::setText(const QString &text)
{
    mLabel.setText(text);
}
