#ifndef SYSTEMTRAY_HPP
#define SYSTEMTRAY_HPP

#include <QSystemTrayIcon>

class systemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    static void showNotification(const QString &title, const QString &msg, MessageIcon icon, int msecs);
    static void init();

private:
    systemTray();
    systemTray(systemTray const&) = delete;
    void operator=(systemTray const&) = delete;

    static systemTray sysTray;

signals:

public slots:

};

#endif // SYSTEMTRAY_HPP
