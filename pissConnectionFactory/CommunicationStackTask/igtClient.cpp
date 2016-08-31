#include "igtClient.h"


/**
 * @brief igtClient::igtClient
 * @param oq
 * @param devices
 */
igtClient::igtClient(int id, QVector <OutputQueue*> *oq, Devices* devices, GlobalTime *globalTime){

    this->id = id;
    this->oq = oq;
    this->devices = devices;
    this->globalTime = globalTime;
    this->motivateConnectionRequest = true;

    //! instante a socket for transmission
    this->soc = new QTcpSocket();

    //! generate correspondance transmission task
    this->transmissionTask = new pissTransmissionTask(this->id, this->oq, devices, soc);

    this->connect(this->soc, SIGNAL(connected()), this, SLOT(startTransfer()));
    this->connect(this->soc,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::connect_request
//! \param addr
//! \param port
//!
qintptr igtClient::connectRequest(QString addr, int port){
    this->motivateConnectionRequest = true;
    this->soc->connectToHost(addr, port);
    return this->soc->socketDescriptor();
}

//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::connectBackRequest
//! \param addr
//! \param port
//!
qintptr igtClient::connectBackRequest(QString addr, int port){
    this->motivateConnectionRequest = false;
    this->soc->connectToHost(addr, port);
    return this->soc->socketDescriptor();
}

//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::slotDisconnected
//!
void igtClient::slotDisconnected(){
    qDebug()<<"disconnected";
}

//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::startTransfer
//!
void igtClient::startTransfer(){
    qDebug()<<"igtClient start transfer using socket: "<<this->soc->socketDescriptor();

    //devices->setSocketTransById(id, soc->socketDescriptor());

    if(motivateConnectionRequest){
        HandShakeMessage *msg = new HandShakeMessage();

        msg->setDataType(1);
        msg->setDeviceId(id);
        msg->setTimestamp(globalTime->currentTime());
        msg->setDLC(38);
        msg->setDeviceName("plateform piss");
        msg->setIP(172, 20, 14, 150);
        msg->setPort(10703);
        msg->print();
        soc->write(msg->toCDatagram());
        soc->flush();
        soc->waitForBytesWritten(-1);
    }
    else{
        HandShakeMessage *msg = new HandShakeMessage();

        msg->setDataType(1);
        msg->setDeviceId(id);
        msg->setTimestamp(globalTime->currentTime());
        msg->setDLC(38);
        msg->setDeviceName("plateform commit");
        msg->setIP(172, 20, 14, 150);
        msg->setPort(10703);
        msg->print();
        soc->write(msg->toCDatagram());
        soc->flush();
        soc->waitForBytesWritten(-1);
//        qDebug()<<"hand shake commit generate";
//        HandShakeCommitMessage *cmsg = new HandShakeCommitMessage();
//        cmsg->setDataType(2);
//        cmsg->setDeviceId(id);
//        cmsg->setTimestamp(globalTime->currentTime());
//        cmsg->setDLC(32);
//        cmsg->setDeviceName("platform");

//        CDatagramme *datagramme = new CDatagramme();
//        //datagramme->setValue(&cmsg->toCDatagram());
//        //qDebug()<<dg.getDataType();

//        this->oq->at(id)->append(datagramme);
//        qDebug()<<"hand shake commit push into :"<<id;
    }

    transmissionTask->launch();
}


