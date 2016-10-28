#include "pissServer.h"


/**
 * @brief pissServer::pissServer
 * @param inputQueueManager
 * @param NetworkEnvironment
 * @param datagrammeAnalyser
 */
pissServer::pissServer(QVector <InputQueue*> *inputQueueManager,
                       QVector <OutputQueue*> *outputQueueManager,
                       Devices *networkEnvironment,
                       DatagrammeAnalyser *datagrammeAnalyser,
                       GlobalTime *globalTime,
                       SystemDataBase* database)
{
    this->serverStatus = false;

    this->inputQueueManager = inputQueueManager;
    this->outputQueueManager = outputQueueManager;
    this->devices = networkEnvironment;
    this->datagrammeAnalyser = datagrammeAnalyser;
    this->globalTime = globalTime;
    this->database = database;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::~pissServer
//!
pissServer::~pissServer(){

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::stopServer
//! \return
//!
bool pissServer::stopServer(){
    if(serverStatus){

        //! TODO stop task related and queues...........

        this->close();
        serverStatus = false;
        return true;
    }
    else{
//        QMessageBox msgBox;
//        msgBox.setText("server do not need to stop, hasn't been launched");
//        msgBox.exec();
        return false;
    }
    return false;
}


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::launchServer
//!
bool pissServer::launchServer(){
    if(serverStatus){
        QMessageBox msgBox;
        msgBox.setText("Server already in listening...");
        msgBox.exec();
        return false;
    }

    serverStatus = this->listen(QHostAddress::Any, this->devices->getMyListenPort());
    return serverStatus;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::getConnectionState
//!
bool pissServer::getConnectionState(){
    return serverStatus;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::incomingConnection
//! \param sd
//!
void pissServer::incomingConnection(qintptr sd){
    qDebug()<<"add incoming device";
    int id = this->devices->addClient();
    this->devices->setSocketReceptionById(id, sd);

    qDebug()<<"generate correspondant input & output queue for the incoming device";
    InputQueue *myInputQueue = new InputQueue();
    OutputQueue *myOutputQueue = new OutputQueue();

    //! push queue pair into global queue manager
    inputQueueManager->push_back(myInputQueue);
    outputQueueManager->push_back(myOutputQueue);

    //! create real time reception task
    pissReceptionTask *receptionTask = new pissReceptionTask(id, sd, datagrammeAnalyser, inputQueueManager, devices);
    receptionTask->start();
}
