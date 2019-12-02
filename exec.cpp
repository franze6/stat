#include "exec.h"

ExecTask::ExecTask(QString pid, QString delay) : QRunnable()
{
    this->pid = pid;
    this->delay = delay;
}

void ExecTask::start()
{
    this->stop = false;
    top = new QProcess();
    grep = new QProcess();
    top->setStandardOutputProcess(grep);
    top->setParent(grep);
    connect(grep, SIGNAL(readyReadStandardOutput()), this, SLOT(dataAvalible()));
    top->start("top -b -p " + this->pid + " -d " + this->delay);
    grep->start("grep "+this->pid);
    grep->waitForFinished(-1);
}

void ExecTask::doStop()
{
    this->stop = true;
}

void ExecTask::run()
{
    start();
}

void ExecTask::killAll()
{
    grep->kill();
    top->kill();
}

QStringList ExecTask::getResults() const
{
    return results;
}

void ExecTask::dataAvalible()
{
    QProcess* proc = (QProcess*)sender();
    while(proc->canReadLine() && !this->stop) {
        this->results << proc->readLine();

    }
    if(this->stop) {
        emit finishedP();
    }
}

QString ExecTask::getPid() const
{
    return pid;
}
