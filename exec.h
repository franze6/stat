#ifndef EXEC_H
#define EXEC_H

#include <QObject>
#include <QProcess>

class Exec : public QObject
{
    Q_OBJECT
public:
    explicit Exec(QString pid, QString delay, QObject *parent = nullptr);
    void doStop();

    QStringList getResults() const;
    QString getPid() const;
public slots:
    void dataAvalible();
    void start();
private:
    QString pid;
    QString delay;
    QStringList results;
    bool stop;

signals:
    void finishedP();
};

#endif // EXEC_H
