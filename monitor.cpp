#include "monitor.h"

Monitor::Monitor(QObject *parent) : QObject(parent)
{
}

void Monitor::startMonitor()
{
    foreach (QString val, this->getPids()) {
        ExecTask* exec = new ExecTask(val, "0,1");
        exec->setAutoDelete(true);
        this->execs.insert(val, exec);
        //connect(thread, SIGNAL(started()), exec, SLOT(start()));
        connect(exec, SIGNAL(finishedP()), this, SLOT(execFinished()));
        QThreadPool::globalInstance()->start(exec);
        //exec->moveToThread(thread);
        //thread->start();
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
    /*foreach(ExecTask* exec, this->execs) {
        exec->doStop();
    }*/
    foreach(ExecTask* exec, this->execs)
    {
        this->list.insert(exec->getPid(), exec->getResults());
        QThreadPool::globalInstance()->clear();
        if(this->list.size() == this->execs.size()) emit monitorFinished();
    }
}

QMap<QString, QStringList> Monitor::getList() const
{
    return list;
}

void Monitor::execFinished()
{
    ExecTask* exec = (ExecTask*)sender();
    this->list.insert(exec->getPid(), exec->getResults());
    if(this->list.size() == this->execs.size()) emit monitorFinished();
}
