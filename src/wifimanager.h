#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include <QTimer>
#include <QIcon>

class WifiManager : public QObject
{
    Q_OBJECT
public:
    explicit WifiManager(QObject *parent = nullptr);
    QString essid() const;
    int signal() const;
    const QIcon& icon() const;

public slots:
    void refresh();

private:
    static QIcon signalToIcon(int signal);

    QTimer mRefreshTimer;
    QString mEssid;
    int mSignal;
    QIcon mIcon;

signals:
    void updated();
};

#endif // WIFIMANAGER_H
