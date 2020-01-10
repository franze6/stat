#include "tcplistener.h"

TcpListener::TcpListener(QObject *parent) : QObject(parent)
{
    this->listener = new QTcpServer(parent);
    this->freq = "0.1";
}

void TcpListener::start(quint16 port)
{
    connect(this->listener, SIGNAL(newConnection()), this, SLOT(newClient()));
    this->listener->listen(QHostAddress::Any, port);
}

void TcpListener::initMonitor()
{
    this->monitor = new Monitor();
    this->monitor->setFreq(this->freq);
    connect(this->monitor, SIGNAL(monitorFinished()), this, SLOT(monitorRes()));
}

void TcpListener::monitorRes()
{
    if(this->results.isEmpty())
        this->results = this->monitor->getList();
    else {
        foreach(QString pid, this->results.keys()) {
            QStringList tmp = this->results.value(pid);
            tmp << this->monitor->getList().value(pid);
            this->results.insert(pid, tmp);
        }

    }
    delete this->monitor;
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
         QTextStream dbg(stdout);
        QString str = socket->readLine().replace("\n", "").replace("\r", "");
        dbg << str;
        if(str.contains("start")) {
            this->initMonitor();
            QString pid = str.split(":").at(1);
            this->monitor->setPids(pid.split(';'));
            this->monitor->startMonitor();
        }
        else if(str.contains("freq")) {
            this->freq = str.split(":").at(1);
        }
        else if(str == "stop") {
            this->monitor->stopMonitor();
        }
        else if(str == "pause") {
            this->monitor->stopMonitor();
        }
        else if(str == "continue") {
            this->initMonitor();
            this->monitor->setPids(QStringList(this->results.keys()));
            this->monitor->startMonitor();
        }
        else if(str == "getresult") {
            QTextStream os(socket);
            QTextStream out(stdout);
            foreach(QString pid, this->results.keys()) {
                foreach(QString val , this->results.value(pid)) {
                    QStringList tmp = val.replace(QRegularExpression("\\s+"), " ").split(' ');
                    os << tmp.at(0) << ":" << tmp.at(5) << ";";
                    out << tmp.at(0) << ":" << tmp.at(5) << ";";
                }
            }
            this->results.clear();
            socket->close();
        }
    }
}
