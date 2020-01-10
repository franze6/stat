#ifndef EXEC_H
#define EXEC_H

#include <QObject>
#include <QProcess>
#include <QRunnable>
#include <QDebug>

class ExecTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit ExecTask(QString pid, QString delay);
    void doStop();
    void run();
    void killAll();
    void restart();

    QStringList getResults() const;
    QString getPid() const;
    QString getDelay() const;
    void setDelay(const QString &value);

public slots:
    void dataAvalible();
    void start();
private:
    QProcess* top;
    QProcess* grep;
    QString pid;
    QString delay;
    QStringList results;
    bool stop;
    bool pause;

signals:
    void finishedP();
};

#endif // EXEC_H
