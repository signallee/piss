#ifndef HANDSHAKEMESSAGE_H
#define HANDSHAKEMESSAGE_H

#include "CDatagramme.h"

/**
 * @brief The HandShakeMessage class
 */
class HandShakeMessage{

public:
    QByteArray toCDatagram();
    void decodeDatagram(CDatagramme* datagram);

    void setDataType(unsigned char dataType);
    void setDeviceId(unsigned char deviceId);
    void setTimestamp(long long timestamp);
    void setDLC(int dlc);
    void setDeviceName(QString name);
    void setIP(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
    void setPort(int port);

    unsigned char getDataType();
    unsigned char getDeviceId();
    unsigned long long getTimestamp();
    int getDLC();
    QString getDeviceName();
    QString getIp();
    int getPort();
    void print();

private:
    unsigned char dataType;
    unsigned char deviceId;
    long long timestamp;
    int dlc;

    char deviceName[20];
    unsigned char ip[4];
    int port;

public:
    HandShakeMessage();
    ~HandShakeMessage();

};

#endif // HANDSHAKEMESSAGE_H

