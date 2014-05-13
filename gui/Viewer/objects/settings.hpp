#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QSettings>

class settings
{
public:
    static void loadSettings();
    static void saveSettings();

    static QVariant getDaemonSettingValue(QString key);
    static void setDaemonSettingValue(QString key, QVariant value);

    static QVariant getMainwindowSettingValue(QString key);
    static void setMainwindowSettingValue(QString key, QVariant value);
private:
    settings();
    settings(settings const&);
    void operator=(settings const&);

    static void loadDaemonSettings();
    static void saveDaemonSettings();

    static void loadMainwindowSettings();
    static void saveMainwindowSettings();

    static settings stgs;
    static QHash<QString, QVariant> daemonStgs;
    static QHash<QString, QVariant> mainWStgs;
};

#endif // SETTINGS_HPP
