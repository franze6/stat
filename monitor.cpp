#include "monitor.h"

Monitor::Monitor(QObject *parent) : QObject(parent)
{
    this->setFreq("0,001");
}

void Monitor::startMonitor()
{
    foreach (QString val, this->getPids()) {
        ExecTask* exec = new ExecTask(val, getFreq());
        exec->setAutoDelete(true);
        this->execs.insert(val, exec);
        connect(exec, SIGNAL(finishedP()), this, SLOT(execFinished()));
        QThreadPool::globalInstance()->start(exec);
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
    foreach(ExecTask* exec, this->execs)
    {
        exec->killAll();
        this->list.insert(exec->getPid(), exec->getResults());
        if(this->list.size() == this->execs.size()) emit monitorFinished();
    }
}

void Monitor::pauseMonitor()
{
        foreach(ExecTask* exec, this->execs)
        {
            exec->killAll();
            this->list.insert(exec->getPid(), exec->getResults());
        }

}

void Monitor::continueMonitor()
{
    foreach (ExecTask* exec, this->execs) {
        exec->restart();
    }
}

QMap<QString, QStringList> Monitor::getList()
{
    return list;
}

QString Monitor::getFreq() const
{
    return freq;
}

void Monitor::setFreq(const QString &value)
{
    freq = value;
}

void Monitor::execFinished()
{
    ExecTask* exec = (ExecTask*)sender();
    this->list.insert(exec->getPid(), exec->getResults());
    if(this->list.size() == this->execs.size()) emit monitorFinished();
}
