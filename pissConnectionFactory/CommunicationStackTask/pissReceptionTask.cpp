#include "pissReceptionTask.h"

/**
 * @brief pissHandShake::pissHandShake
 * @param SD
 * @param datagrammeAnalyser
 * @param inputQueueManager
 * @param networkEnvironment
 */
pissReceptionTask::pissReceptionTask(int id,
                             qintptr SD,
                             DatagrammeAnalyser *datagrammeAnalyser,
                             QVector <InputQueue*> *inputQueueManager,
                             Devices *networkEnvironment)
{
    this->id = id;
    this->sd = SD;
    this->blockSize = 0;
    this->AIM = 0;
    this->transType = 0;
    this->DLC = 0;
    this->bytesReceived = 0;
    this->totalBytes = 0;
    this->datagrammeAnalyser = datagrammeAnalyser;
    this->inputQueueManager = inputQueueManager;
    this->networkEnvironment = networkEnvironment;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::~pissHandShake
//!
pissReceptionTask::~pissReceptionTask(){

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::run
//!
void pissReceptionTask::run()
{
    this->soc = new QTcpSocket();
    if(!this->soc->setSocketDescriptor(this->sd))
    {
        return;// error msg
    }

    this->connect(this->soc, SIGNAL(readyRead()), this, SLOT(lecture()));
    //connect(this->soc, SIGNAL(disconnected()), this, SLOT(disconnect()));
    exec();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::lecture
//!
void pissReceptionTask::lecture()
{
    qDebug()<<"new message arrive";
    QByteArray input = this->soc->readAll();
    CDatagramme *datagramme = new CDatagramme();
    datagramme->setValue(&input);

    this->mutexInputqueueManager.lock();
    this->inputQueueManager->at(this->id)->append(datagramme);
    this->mutexInputqueueManager.unlock();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::disconnect
//!
void pissReceptionTask::disconnect()
{
    this->soc->deleteLater();
}
