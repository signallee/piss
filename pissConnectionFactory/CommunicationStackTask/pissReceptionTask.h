#ifndef PISSHANDSHAKE_H
#define PISSHANDSHAKE_H

#include<QThread>
#include<QDebug>
#include<QTcpSocket>
#include<QDataStream>
#include<QString>
#include <QMutex>
#include"DatagrammeAnalyser.h"
#include "CDatagramme.h"


class pissReceptionTask : public QThread
{
    Q_OBJECT
public:
    pissReceptionTask(int id,
                    qintptr SD,
                    DatagrammeAnalyser *datagrammeAnalyser,
                    QVector <InputQueue*> *inputQueueManager,
                    Devices *networkEnvironment);//socket descriptor
    ~pissReceptionTask();
    void run();

private:
    int id;
    int clientCount;
    DatagrammeAnalyser *datagrammeAnalyser;
    QVector <InputQueue*> *inputQueueManager;
    Devices *networkEnvironment;
    qintptr sd;
    QTcpSocket *soc;
    qint16 blockSize;
    QString message;

    quint16 AIM;
    quint16 transType;
    quint16 DLC;
    QString currentTime;

    qint16 bytesReceived;
    qint16 totalBytes;
    QMutex mutexInputqueueManager;

public slots:
    void lecture();
    void disconnect();
};

#endif // PISSHANDSHAKE_H
