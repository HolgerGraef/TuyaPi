#include "wifimanager.h"

#include <QProcess>

#include "main.h"

WifiManager::WifiManager(QObject* parent)
  : QObject{ parent }
  , mEssid("")
  , mSignal(-100)
{
  mRefreshTimer.setInterval(1000);
  connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
  mRefreshTimer.start();
  refresh();
}

QString
WifiManager::essid() const
{
  return mEssid;
}

int
WifiManager::signal() const
{
  return mSignal;
}

const QIcon&
WifiManager::icon() const
{
  return mIcon;
}

QString
WifiManager::iconString() const
{
  return signalToIconString(mSignal);
}

void
WifiManager::refresh()
{
  QString essidLine;
  QString signalLine;

  auto process = new QProcess();
  process->start("/usr/sbin/iwconfig", { "wlan0" });
  process->waitForFinished(1000);
  while (process->canReadLine()) {
    const auto& line = QString::fromUtf8(process->readLine());
    if (line.contains("ESSID")) {
      essidLine = line;
    } else if (line.contains("Signal level")) {
      signalLine = line;
    }
  }

  if (essidLine.contains(":")) {
    mEssid = essidLine.split(":")[1].trimmed();

    /* remove inverted commas */
    mEssid.chop(1);
    mEssid.remove(0, 1);
  } else {
    mEssid = "";
  }

  if (signalLine.count("=") > 1) {
    mSignal = signalLine.split("=")[2].split(" ")[0].toInt();
  } else {
    mSignal = -100;
  }

  mIcon = signalToIcon(mSignal);

  emit updated();
}

int
WifiManager::signalToCharCode(int signal)
{
  if (signal >= -30)
    return fa::mdi6_wifi_strength_4;
  else if (signal >= -50)
    return fa::mdi6_wifi_strength_3;
  else if (signal >= -67)
    return fa::mdi6_wifi_strength_2;
  else if (signal >= -99)
    return fa::mdi6_wifi_strength_1;
  return fa::mdi6_wifi_strength_off_outline;
}

QIcon
WifiManager::signalToIcon(int signal)
{
  return awesome()->icon(fa::mdi6, signalToCharCode(signal));
}

QString
WifiManager::signalToIconString(int signal)
{
  return QString::fromStdU32String(
    std::u32string({ static_cast<char32_t>(signalToCharCode(signal)) }));
}
