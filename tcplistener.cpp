#include "tcplistener.h"

TcpListener::TcpListener(QObject *parent) : QObject(parent)
{
    this->listener = new QTcpServer(parent);
    this->monitor = new Monitor(parent);
    connect(this->monitor, SIGNAL(monitorFinished()), this, SLOT(monitorRes()));
}

void TcpListener::start(quint16 port)
{
    connect(this->listener, SIGNAL(newConnection()), this, SLOT(newClient()));
    this->listener->listen(QHostAddress::Any, port);
}

void TcpListener::monitorRes()
{
    QTextStream os(stdout);
    foreach(QString pid, this->monitor->getList().keys()) {
        foreach(QString val , this->monitor->getList().value(pid)) {
            os << pid << ":" << val.replace(QRegularExpression("\\s+"), " ").split(' ').at(6) << endl;
        }
    }
}

void TcpListener::newClient()
{
    if(this->listener->hasPendingConnections()) {
        QTcpSocket* soket = this->listener->nextPendingConnection();
        connect(soket, SIGNAL(readyRead()), this, SLOT(clientDataRead()));
    }
}

void TcpListener::clientDataRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    while(socket->canReadLine()) {
        QString str = socket->readLine().replace("\r\n", "");
        if(str == "start") {
            socket->waitForReadyRead();
            QString pid = socket->readLine().replace("\r\n", "");
            this->monitor->setPids(pid.split(';'));
            this->monitor->startMonitor();
        }
        else if(str == "stop") {
            this->monitor->stopMonitor();

        }
        else if(str == "getresult") {
            QTextStream os(socket);
            foreach(QString pid, this->monitor->getList().keys()) {
                foreach(QString val , this->monitor->getList().value(pid)) {
                    os << pid << ":" << val.replace(QRegularExpression("\\s+"), " ").split(' ').at(6) << endl;
                }
            }
        }
    }
}
