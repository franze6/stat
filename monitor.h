#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include "executor.h"
#include <QThread>
#include <QMap>

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = nullptr);
    void startMonitor();
    QStringList getPids() const;
    void setPids(const QStringList &value);

    void stopMonitor();

    QMap<QString, QStringList> getList() const;

private:
    QStringList pids;
    QMap<QString, Exec*> execs;
    QMap<QString, QStringList> list;

signals:
    void monitorFinished();

public slots:
    void execFinished();
};

#endif // MONITOR_H
