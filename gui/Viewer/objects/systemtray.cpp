#include "systemtray.hpp"

systemTray systemTray::sysTray;

systemTray::systemTray() : QSystemTrayIcon()
{

}

void systemTray::init()
{
    sysTray.setIcon(QPixmap(":/icons/salamandre.png"));
    sysTray.setVisible(true);
    sysTray.show();
}

void systemTray::showNotification(const QString &title, const QString &msg, MessageIcon icon, int msecs)
{
    sysTray.showMessage(title, msg, icon, msecs);
}
