#ifndef IGTCLIENT_H
#define IGTCLIENT_H

#include <QTcpSocket>

#include "pissTransmissionTask.h"
#include "HandShakeMessage.h"
#include "Devices.h"
#include "Device.h"
#include "GlobalTime.h"
#include "HandShakeCommitMessage.h"

class igtClient : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* soc;

    pissTransmissionTask *transmissionTask;
    QVector <OutputQueue*> *oq;
    Devices* devices;
    bool motivateConnectionRequest;
    int id;
    GlobalTime *globalTime;
public:
    qintptr connectRequest(QString addr, int port);
    qintptr connectBackRequest(QString addr, int port);

public slots:
    void startTransfer();
    void slotDisconnected();

public:
    igtClient(int id, QVector <OutputQueue*> *oq, Devices* devices, GlobalTime *globalTime);

};

#endif // IGTCLIENT_H

