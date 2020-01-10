#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include "exec.h"
#include <QThreadPool>
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
    void pauseMonitor();
    void continueMonitor();

    QMap<QString, QStringList> getList();
    QString getFreq() const;
    void setFreq(const QString &value);

private:
    QStringList pids;
    QString freq;
    QMap<QString, ExecTask*> execs;
    QMap<QString, QStringList> list;

signals:
    void monitorFinished();

public slots:
    void execFinished();
};

#endif // MONITOR_H
