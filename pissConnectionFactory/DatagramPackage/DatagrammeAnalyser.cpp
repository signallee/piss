#include "DatagrammeAnalyser.h"


DatagrammeAnalyser::DatagrammeAnalyser(QVector <OutputQueue*> *oq,
                                       QVector <InputQueue*> *iq,
                                       Devices* environment,
                                       GlobalTime *globalTime,
                                       SystemDataBase* database)
{
    this->iq = iq;
    this->oq = oq;
    qDebug()<<"DA"<<oq;
    this->devices = environment;
    this->globalTime = globalTime;
    this->database = database;
}

DatagrammeAnalyser::~DatagrammeAnalyser()
{

}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::encodage
//! \param modulename
//!
void DatagrammeAnalyser::encodage(QString modulename)
{

}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodage
//! \param id
//! \param datagramme
//!
void DatagrammeAnalyser::decodage(int id, CDatagramme *datagramme){

    switch (datagramme->getDataType()){
        case HelloMessage: {
            qDebug()<<"HelloMessage";
            decodeHelloMessage(id, datagramme);
            break;
        }
        case HandShakeMsg:{
            qDebug()<<"HandShakeMessage";
            decodeHandShakeMessage(id, datagramme);
            break;
        }
        case HandShakeCommitMsg:{
            qDebug()<<"HandShakeCommitMsg";
            decodeHandShakeCommitMessage(id, datagramme);
            break;
        }
        case CTImage:{
            qDebug()<<"CTImage";
            decodeCTImage(datagramme);
            break;
        }
    }
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeHelloMessage
//! \param id
//! \param datagramme
//!
void DatagrammeAnalyser::decodeHelloMessage(int id, CDatagramme *datagramme){
    datagramme->setTimestamp(globalTime->currentTime());
    oq->at(id)->append(datagramme);
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeHandShakeMessage
//! \param id
//! \param datagramme
//!
void DatagrammeAnalyser::decodeHandShakeMessage(int id, CDatagramme *datagramme){

    HandShakeMessage *msg = new HandShakeMessage();
    msg->decodeDatagram(datagramme);
    msg->print();

    this->devices->setClientlistenportById(id, msg->getPort());

    emit handshakeMessageReactProcess(msg->getIp(), msg->getPort());

}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeHandShakeCommitMessage
//! \param id
//! \param datagramme
//!
void DatagrammeAnalyser::decodeHandShakeCommitMessage(int id, CDatagramme *datagramme){
    HandShakeCommitMessage *msg = new HandShakeCommitMessage();
    msg->decodeDatagram(datagramme);

    //devices->setSocketTransById(id, waitingList.at(id).second);
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeCTImage
//! \param datagramme
//!
void DatagrammeAnalyser::decodeCTImage(CDatagramme *datagramme){

}

void DatagrammeAnalyser::setConnectBackRequestWaitingPair(int waitingId, qintptr waitingSocket){
    QPair<int, qintptr> pair;
    pair.first = waitingId;
    pair.second = waitingSocket;
    waitingList.append(pair);
}
