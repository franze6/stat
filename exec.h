#ifndef EXEC_H
#define EXEC_H

#include <QObject>
#include <QProcess>
#include <QRunnable>

class ExecTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit ExecTask(QString pid, QString delay);
    void doStop();
    void run() override;

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
