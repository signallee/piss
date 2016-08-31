#include "HandShakeCommitMessage.h"

HandShakeCommitMessage::HandShakeCommitMessage()
{
}

HandShakeCommitMessage::~HandShakeCommitMessage(){

}

void HandShakeCommitMessage::setDataType(unsigned char dataType){
    this->dataType = dataType;
}

void HandShakeCommitMessage::setDeviceId(unsigned char deviceId){
    this->deviceId = deviceId;
}

void HandShakeCommitMessage::setTimestamp(long long timestamp){
    this->timestamp = timestamp;
}

void HandShakeCommitMessage::setDLC(int dlc){
    this->dlc = dlc;
}

void HandShakeCommitMessage::setDeviceName(QString name){
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

unsigned char HandShakeCommitMessage::getDataType(){
    return this->dataType;
}

unsigned char HandShakeCommitMessage::getDeviceId(){
    return this->deviceId;
}

unsigned long long HandShakeCommitMessage::getTimestamp(){
    return this->timestamp;
}

int HandShakeCommitMessage::getDLC(){
    return this->dlc;
}

QString HandShakeCommitMessage::getDeviceName(){
    QByteArray name;
    name.resize(20);
    for(int i = 0; i < 20; i++){
        name[i] = this->deviceName[i];
    }
    QString dname;
    dname.prepend(name);

    return dname.trimmed();
}

QByteArray HandShakeCommitMessage::toCDatagram(){
    QByteArray buf;
    buf.resize(32);
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

    return buf;
}

void HandShakeCommitMessage::decodeDatagram(CDatagramme *datagram){

    this->setDataType(datagram->getDataType());
    this->setDeviceId(datagram->getDeviceId());
    this->setTimestamp(datagram->getTimestamp());
    this->setDLC(datagram->getDLC());
    this->setDeviceName(datagram->getValue()->mid(12, 20));
}
