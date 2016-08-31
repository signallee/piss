#ifndef PISSINPUTINFORMATIONDECODER_H
#define PISSINPUTINFORMATIONDECODER_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include "DatagrammeAnalyser.h"
#include "CDatagramme.h"

class pissInputInformationDecoder : public QThread
{
public:
    pissInputInformationDecoder(QVector <InputQueue*> *inputQueueManager,
                                Devices* NetworkEnvironment,
                                DatagrammeAnalyser *datagrammeAnalyser);
    ~pissInputInformationDecoder();
    void run();
    void stop();

private:
    Devices *networkEnvironment;
    QVector <InputQueue*> *inputQueueManager;
    DatagrammeAnalyser *datagrammeAnalyser;
    CDatagramme data;
    QMutex inputMutex;

    bool flag;
};

#endif // PISSINPUTINFORMATIONDECODER_H
