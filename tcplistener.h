#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include "monitor.h"
#include <QRegularExpression>
class TcpListener : public QObject
{
    Q_OBJECT
public:
    explicit TcpListener(QObject *parent = nullptr);
    void start(quint16 port);
    QTcpServer* listener;

private:
    Monitor* monitor;
    void initMonitor();
    QMap<QString, QStringList> results;
    QString freq;

signals:

public slots:
    void monitorRes();
    void newClient();
    void clientDataRead();
};

#endif // TCPLISTENER_H
