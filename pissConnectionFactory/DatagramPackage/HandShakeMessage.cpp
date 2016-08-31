#include "HandShakeMessage.h"

HandShakeMessage::HandShakeMessage()
{
}

HandShakeMessage::~HandShakeMessage(){

}

void HandShakeMessage::print(){
    qDebug()<<this->dataType<<this->deviceId<<this->timestamp<<this->dlc<<this->deviceName<<ip[0]<<ip[1]<<ip[2]<<ip[3]<<port;
}

void HandShakeMessage::setDataType(unsigned char dataType){
    this->dataType = dataType;
}

void HandShakeMessage::setDeviceId(unsigned char deviceId){
    this->deviceId = deviceId;
}

void HandShakeMessage::setTimestamp(long long timestamp){
    this->timestamp = timestamp;
}

void HandShakeMessage::setDLC(int dlc){
    this->dlc = dlc;
}

void HandShakeMessage::setDeviceName(QString name){
    QByteArray qba = name.toLatin1();

    if(qba.size() <= 20){
        for(int j = 0; j < qba.size(); j++){
            this->deviceName[j] = char(qba.at(j));
        }

        for(int k = 19; k > qba.size()-1; k--){
            this->deviceName[k] = char(' ');
        }
    }
}

void HandShakeMessage::setIP(unsigned char a, unsigned char b, unsigned char c, unsigned char d){
    this->ip[0] = a;
    this->ip[1] = b;
    this->ip[2] = c;
    this->ip[3] = d;
}

void HandShakeMessage::setPort(int port){
    this->port = port;
}

unsigned char HandShakeMessage::getDataType(){
    return this->dataType;
}

unsigned char HandShakeMessage::getDeviceId(){
    return this->deviceId;
}

unsigned long long HandShakeMessage::getTimestamp(){
    return this->timestamp;
}

int HandShakeMessage::getDLC(){
    return this->dlc;
}

QString HandShakeMessage::getDeviceName(){
    QByteArray name;
    name.resize(20);
    for(int i = 0; i < 20; i++){
        name[i] = this->deviceName[i];
    }
    QString dname;
    dname.prepend(name);

    return dname.trimmed();
}

QString HandShakeMessage::getIp(){    
    return    QString::number((unsigned char)(this->ip[0])) + "."
            + QString::number((unsigned char)(this->ip[1])) + "."
            + QString::number((unsigned char)(this->ip[2])) + "."
            + QString::number((unsigned char)(this->ip[3]));
}

int HandShakeMessage::getPort(){
    return this->port;
}

QByteArray HandShakeMessage::toCDatagram(){
    QByteArray buf;
    buf.resize(38);
    buf[0] = dataType;

    buf[1] = deviceId;

    buf[2] = timestamp % (long long)(pow(2, 8));
    buf[3] = timestamp / (long long)(pow(2, 8)) % (long long)(pow(2, 8));
    buf[4] = timestamp / (long long)(pow(2, 16)) % (long long)(pow(2,8));
    buf[5] = timestamp / (long long)(pow(2, 24)) % (long long)(pow(2,8));
    buf[6] = timestamp / (long long)(pow(2, 32)) % (long long)(pow(2,8));
    buf[7] = timestamp / (long long)(pow(2, 40)) % (long long)(pow(2,8));
    buf[8] = timestamp / (long long)(pow(2, 56)) % (long long)(pow(2,8));
    buf[9] = timestamp / (long long)(pow(2, 64)) % (long long)(pow(2,8));

    buf[10] = (uchar)  (0x000000ff & dlc);
    buf[11] = (uchar) ((0x0000ff00 & dlc) >> 8);

    for(unsigned cpt = 0 ; cpt < 20; cpt++){
        buf[12 + cpt] = deviceName[cpt];
    }

    buf[32] = ip[0];
    buf[33] = ip[1];
    buf[34] = ip[2];
    buf[35] = ip[3];

    buf[36] = (uchar)  (0x000000ff & port);
    buf[37] = (uchar) ((0x0000ff00 & port) >> 8);

    return buf;
}

void HandShakeMessage::decodeDatagram(CDatagramme *datagram){
    this->setDataType(datagram->getDataType());
    this->setDeviceId(datagram->getDeviceId());
    this->setTimestamp(datagram->getTimestamp());
    this->setDLC(datagram->getDLC());
    this->setDeviceName(datagram->getValue()->mid(12, 20));
    this->setIP((unsigned char)(datagram->getValue()->at(32)),
                (unsigned char)(datagram->getValue()->at(33)),
                (unsigned char)(datagram->getValue()->at(34)),
                (unsigned char)(datagram->getValue()->at(35)));
    this->setPort((unsigned char)(datagram->getValue()->at(37))*256 + (unsigned char)(datagram->getValue()->at(36)));
}

