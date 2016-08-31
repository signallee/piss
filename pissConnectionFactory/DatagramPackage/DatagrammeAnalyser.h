#ifndef DATAGRAMMEANALYSER_H
#define DATAGRAMMEANALYSER_H

#include <QHostAddress>
#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QByteArray>
#include <QPair>
#include <QList>
#include "InputQueue.h"
#include "OutputQueue.h"
#include "CDatagramme.h"
#include "Devices.h"
#include "pissTransmissionTask.h"
#include "GlobalTime.h"
#include "igtClient.h"
#include "SystemDataBase.h"
#include "HandShakeMessage.h"
#include "HandShakeCommitMessage.h"

//! Codes des erreurs remontees par les fonctions
typedef enum {
    HelloMessage = 0,
    HandShakeMsg,
    HandShakeCommitMsg,
    CTImage
}DatagrammeIdentifierCode;

/**
 * @brief The DatagrammeAnalyser class
 *
 */
class DatagrammeAnalyser:public QObject
{
    Q_OBJECT
public:
    DatagrammeAnalyser(QVector <OutputQueue*> *oq, QVector <InputQueue*> *iq, Devices *environment, GlobalTime *globalTime, SystemDataBase* database);
    ~DatagrammeAnalyser();

    void encodage(QString modulename);
    void decodage(int id, CDatagramme *datagramme);

    void decodeHelloMessage(int id, CDatagramme *datagramme);
    void decodeHandShakeMessage(int id, CDatagramme *datagramme);
    void decodeHandShakeCommitMessage(int id, CDatagramme *datagramme);
    void decodeCTImage(CDatagramme *datagramme);
    void setConnectBackRequestWaitingPair(int waitingId, qintptr waitingSocket);
    //int getDatagrammeLength();

signals:
    void handshakeMessageReactProcess(QString ip, int port);

private:
    DatagrammeIdentifierCode identifierCode;

    QVector <InputQueue*> *iq;
    QVector <OutputQueue*> *oq;

    Devices *devices;
    GlobalTime *globalTime;

    SystemDataBase* database;

    QList<QPair<int, qintptr> > waitingList;
};

#endif // DATAGRAMMEANALYSER_H
