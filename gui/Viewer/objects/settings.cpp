#include "settings.hpp"

#include <QDir>
#include <QDebug>

settings settings::stgs = settings();
QHash<QString, QVariant> settings::daemonStgs = QHash<QString, QVariant>();
QHash<QString, QVariant> settings::mainWStgs = QHash<QString, QVariant>();

settings::settings(){
}

void settings::loadSettings()
{
    settings::loadDaemonSettings();
    settings::loadMainwindowSettings();
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

void settings::loadMainwindowSettings()
{
    QSettings s(QSettings::IniFormat, QSettings::UserScope, "Salamandre", "Viewer");
    s.beginGroup("mainwStgs");

    QStringList keys = s.allKeys();
    int nbKeys = keys.size();

    for(int i = 0; i < nbKeys; ++i){
        stgs.mainWStgs.insert(keys.at(i), s.value(keys.at(i)));
    }

    s.endGroup();
}

void settings::saveSettings()
{
    settings::saveDaemonSettings();
    settings::saveMainwindowSettings();;
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

void settings::saveMainwindowSettings()
{
    QSettings s(QSettings::IniFormat, QSettings::UserScope, "Salamandre", "Viewer");
    s.beginGroup("mainwStgs");

    for(auto it = stgs.mainWStgs.begin(); it != stgs.mainWStgs.end(); ++it){
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

QVariant settings::getMainwindowSettingValue(QString key)
{
    return stgs.mainWStgs.value(key);
}

void settings::setMainwindowSettingValue(QString key, QVariant value)
{
    stgs.mainWStgs.insert(key, value);
}
