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
    qDebug() << this->monitor->getList();
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
            QString pid = socket->readLine().replace("\r\n", "");
            socket->waitForReadyRead();
            this->monitor->setPids(QStringList() << pid);
            this->monitor->startMonitor();
        }
        else if(str == "stop") {
            this->monitor->stopMonitor();

        }
    }
}
