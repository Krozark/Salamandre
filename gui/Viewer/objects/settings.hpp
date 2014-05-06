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
private:
    settings();
    settings(settings const&);
    void operator=(settings const&);

    static void loadDaemonSettings();
    static void saveDaemonSettings();

    static settings stgs;
    static QHash<QString, QVariant> daemonStgs;
};

#endif // SETTINGS_HPP
