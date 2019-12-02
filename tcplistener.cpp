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
        QString str = socket->readLine().replace("\n", "").replace("\r", "");
        if(str.contains("start")) {
            QString pid = str.split(":").at(1);
            this->monitor->setPids(pid.split(';'));
            this->monitor->startMonitor();
        }
        else if(str.contains("freq")) {
            QString freq = str.split(":").at(1);
            this->monitor->setFreq(freq);
        }
        else if(str == "stop") {
            this->monitor->stopMonitor();
        }
        else if(str == "getresult") {
            QTextStream os(socket);
            QTextStream out(stdout);
            foreach(QString pid, this->monitor->getList().keys()) {
                foreach(QString val , this->monitor->getList().value(pid)) {
                    QStringList tmp = val.replace(QRegularExpression("\\s+"), " ").split(' ');
                    os << tmp.at(1) << ":" << tmp.at(6) << ";";
                    out << tmp.at(1) << ":" << tmp.at(6) << ";";
                }
            }
            socket->close();
        }
    }
}
