#ifndef THREADSAVE_HPP
#define THREADSAVE_HPP

#include <QThread>

class threadSave : public QThread
{
    Q_OBJECT
public:
    explicit threadSave(QObject *parent = 0);

signals:

public slots:

};

#endif // THREADSAVE_HPP
