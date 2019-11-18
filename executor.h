#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <QProcess>
#include <QObject>


class Exec : public QObject
{
    Q_OBJECT
public:
    explicit Exec(QString pid, QString delay, QObject *parent = nullptr);
    void start();
    void doStop();

    QStringList getResults() const;
    QString getPid() const;

public slots:
    void dataAvalible();
private:
    QString pid;
    QString delay;
    QStringList results;
    bool stop;

signals:
    void finishedP();


};

#endif // EXECUTOR_H
