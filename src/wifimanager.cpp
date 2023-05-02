#include "wifimanager.h"
#include <QProcess>
#include "main.h"

WifiManager::WifiManager(QObject *parent)
    : QObject{parent},
      mEssid(""),
      mSignal(-100)
{
    mRefreshTimer.setInterval(1000);
    connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    mRefreshTimer.start();
    refresh();
}

QString WifiManager::essid() const
{
    return mEssid;
}

int WifiManager::signal() const
{
    return mSignal;
}

const QIcon& WifiManager::icon() const
{
    return mIcon;
}

void WifiManager::refresh()
{
    QString essidLine;
    QString signalLine;

    auto process = new QProcess();
    process->start("/usr/sbin/iwconfig", {"wlan0"});
    process->waitForFinished(1000);
    while(process->canReadLine()) {
        const auto& line = QString::fromUtf8(process->readLine());
        if(line.contains("ESSID")) {
            essidLine = line;
        } else if(line.contains("Signal level")) {
            signalLine = line;
        }
    }

    if(essidLine.contains(":")) {
        mEssid = essidLine.split(":")[1].trimmed();

        /* remove inverted commas */
        mEssid.chop(1);
        mEssid.remove(0, 1);
    } else {
        mEssid = "";
    }

    if(signalLine.count("=") > 1) {
        mSignal = signalLine.split("=")[2].split(" ")[0].toInt();
    } else {
        mSignal = -100;
    }

    mIcon = signalToIcon(mSignal);

    emit updated();
}

QIcon WifiManager::signalToIcon(int signal)
{
    if(signal >= -50)
        return awesome()->icon(fa::fa_solid, fa::fa_wifi_strong);
    else if(signal >= -67)
        return awesome()->icon(fa::fa_solid, fa::fa_wifi_3);
    return awesome()->icon(fa::fa_solid, fa::fa_wifi);
}
