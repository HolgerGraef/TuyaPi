#include "main.h"
#include "mainwidget.h"
#include <QApplication>

#include <tuyacpp/scanner.hpp>
#include <tuyacpp/device.hpp>

static fa::QtAwesome* mAwesome = nullptr;

static void makeQtAwesome()
{
    mAwesome = new fa::QtAwesome(qApp);
    mAwesome->initFontAwesome();
}

fa::QtAwesome* awesome()
{
    return mAwesome;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    tuya::Loop loop;
    tuya::Scanner scanner(loop);
    scanner.scan();

    return 0;

    makeQtAwesome();
    MainWidget w;
    w.show();
    return a.exec();
}
