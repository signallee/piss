#ifndef HANDSHAKECOMMITMESSAGE_H
#define HANDSHAKECOMMITMESSAGE_H

#include "CDatagramme.h"

/**
 * @brief The HandShakeCommitMessage class
 */
class HandShakeCommitMessage{

public:
    QByteArray toCDatagram();
    void decodeDatagram(CDatagramme* datagram);

    void setDataType(unsigned char dataType);
    void setDeviceId(unsigned char deviceId);
    void setTimestamp(long long timestamp);
    void setDLC(int dlc);
    void setDeviceName(QString name);

    unsigned char getDataType();
    unsigned char getDeviceId();
    unsigned long long getTimestamp();
    int getDLC();
    QString getDeviceName();

private:
    unsigned char dataType;
    unsigned char deviceId;
    unsigned long long timestamp;
    int dlc;
    char deviceName[20];

public:
    HandShakeCommitMessage();
    ~HandShakeCommitMessage();

};

#endif // HANDSHAKECOMMITMESSAGE_H
