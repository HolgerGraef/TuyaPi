#include "main.h"

#include <QApplication>

#include "mainwidget.h"

#if !defined(TUYAPI_IS_DESKTOP)
#define TUYAPI_IS_DESKTOP 0
#endif

static fa::QtAwesome* mAwesome = nullptr;

static void
makeQtAwesome()
{
  mAwesome = new fa::QtAwesome(qApp);
  mAwesome->initFontAwesome();
}

fa::QtAwesome*
awesome()
{
  return mAwesome;
}

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  makeQtAwesome();
  MainWidget w(TUYAPI_IS_DESKTOP);
  w.show();
  return a.exec();
}
