#include "settings.hpp"

#include <QDir>
#include <QDebug>

settings settings::stgs = settings();
QHash<QString, QVariant> settings::daemonStgs = QHash<QString, QVariant>();

settings::settings(){
}

void settings::loadSettings()
{
    settings::loadDaemonSettings();
}

void settings::loadDaemonSettings()
{
    QSettings s(QSettings::IniFormat, QSettings::UserScope, "Salamandre", "Viewer");
    s.beginGroup("daemonSrv");

    QStringList keys = s.allKeys();
    int nbKeys = keys.size();

    for(int i = 0; i < nbKeys; ++i){
        stgs.daemonStgs.insert(keys.at(i), s.value(keys.at(i)));
    }

    s.endGroup();
}

void settings::saveSettings()
{
    settings::saveDaemonSettings();
    qDebug() << "save settings";
}

void settings::saveDaemonSettings()
{
    QSettings s(QSettings::IniFormat, QSettings::UserScope, "Salamandre", "Viewer");
    s.beginGroup("daemonSrv");

    for(auto it = stgs.daemonStgs.begin(); it != stgs.daemonStgs.end(); ++it){
        s.setValue(it.key(), it.value());

        qDebug() << "insert val : " << it.key() << " : " << it.value();
    }

    s.endGroup();
}

QVariant settings::getDaemonSettingValue(QString key)
{
    return stgs.daemonStgs.value(key);
}

void settings::setDaemonSettingValue(QString key, QVariant value)
{
    stgs.daemonStgs.insert(key, value);
}
