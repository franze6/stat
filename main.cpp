#include <QCoreApplication>
#include "tcplistener.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TcpListener* listener = new TcpListener();
    listener->start(33333);

    return a.exec();
}
