#include "exec.h"

Exec::Exec(QString pid, QString delay, QObject *parent) : QObject(parent)
{
    this->pid = pid;
    this->delay = delay;
}

void Exec::start()
{
    this->stop = false;
    QProcess top;
    QProcess grep;
    top.setStandardOutputProcess(&grep);
    connect(&grep, SIGNAL(readyReadStandardOutput()), this, SLOT(dataAvalible()));
    top.start("top -b -p " + this->pid + " -d " + this->delay);
    grep.start("grep "+this->pid);
}

void Exec::doStop()
{
    this->stop = true;
}

QStringList Exec::getResults() const
{
    return results;
}

void Exec::dataAvalible()
{
    QProcess* proc = (QProcess*)sender();
    while(proc->canReadLine() && !this->stop) {
        this->results << proc->readLine();
    }
    if(this->stop) {
        proc->kill();
        emit finishedP();
    }
}

QString Exec::getPid() const
{
    return pid;
}
