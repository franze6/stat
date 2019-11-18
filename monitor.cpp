#include "monitor.h"

Monitor::Monitor(QObject *parent) : QObject(parent)
{
}

void Monitor::startMonitor()
{
    foreach (QString val, this->getPids()) {
        Exec* exec = new Exec(val, "0,001", this);
        this->execs.insert(val, exec);
        QThread *thread = new QThread();
        connect(thread, SIGNAL(started()), exec, SLOT(start()));
        connect(exec, SIGNAL(finishedP()), this, SLOT(execFinished()));
        exec->moveToThread(thread);
        thread->start();
    }
}

QStringList Monitor::getPids() const
{
    return pids;
}

void Monitor::setPids(const QStringList &value)
{
    pids = value;
}

void Monitor::stopMonitor()
{
    foreach(Exec* exec, this->execs) {
        exec->doStop();
    }
}

QMap<QString, QStringList> Monitor::getList() const
{
    return list;
}

void Monitor::execFinished()
{
    Exec* exec = (Exec*)sender();
    this->list.insert(exec->getPid(), exec->getResults());
    if(this->list.size() == this->execs.size()) emit monitorFinished();
}
