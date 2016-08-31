#ifndef PISSTRANSMISSIONTASK_H
#define PISSTRANSMISSIONTASK_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "Devices.h"
#include "OutputQueue.h"


class pissTransmissionTask : public QThread
{
    Q_OBJECT

public:
    void launch();
    void run();

private:
    QTcpSocket* transmissionSocket;
    QVector <OutputQueue*> *oq;
    Devices *devices;
    QTimer *transmissionTimer;
    int frameCounter;
    QMutex outputMutex;
    int id;

public:
    pissTransmissionTask(int id, QVector <OutputQueue*> *oq, Devices* devices, QTcpSocket* transmissionSocket);
    ~pissTransmissionTask();

public slots:
    void transfer();
};

#endif // PISSTRANSMISSIONTASK_H
