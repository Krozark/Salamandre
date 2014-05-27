#ifndef THREADGESTIONAPPS_HPP
#define THREADGESTIONAPPS_HPP

#include <QThread>

class threadGestionApps : public QThread
{
    Q_OBJECT
public:
    explicit threadGestionApps(QObject *parent = 0);

    void run();
    void startApps(int srvGuiPort);
    void stopApps();

private:
    enum cmdApps{
        START_APPS = 0,
        STOP_APPS
    };

    cmdApps currentCmd;
    int srvGuiPort;

    int _startApps();
    void _stopApps();
signals:
    void connectionSuccess();
    void connectionFailed(int);
    void deconnectionSuccess();
    void deconnectionFailed();
    void askFileDaemon();

public slots:

};

#endif // THREADGESTIONAPPS_HPP
