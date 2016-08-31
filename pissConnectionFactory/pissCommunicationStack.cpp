#include "pissCommunicationStack.h"


/**
 * @brief pissCommunicationStack::pissCommunicationStack
 *
 * The communication stack include four components and three permanent real time task. each time when an incoming device connect local device via protocole tcp/ip,
 *
 * a pair of task(reception&transmission) wil be generated, at the same time, a pair of datagram queue(input&ouput) will be push into related global datagrame queue manager.
 *
 * @param globalTime
 */
pissCommunicationStack::pissCommunicationStack(GlobalTime *globalTime){

    //! get global timer's pointer
    this->globalTime = globalTime;

    //! procedure to generate the architecture of the real time communication
    this->initComponents();
    this->initRealTimeTasks();
    this->setConnections();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::setConnections
//!
void pissCommunicationStack::setConnections(){
    this->connect(this->datagrammeAnalyser, SIGNAL(handshakeMessageReactProcess(QString,int)), this, SLOT(connectBackRequest(QString,int)));
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::initComponents
//!
void pissCommunicationStack::initComponents(){
    this->devices = new Devices();
    this->devices->setMyListenPort(10703);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::initRealTimeTasks
//!
void pissCommunicationStack::initRealTimeTasks(){
    this->datagrammeAnalyser = new DatagrammeAnalyser(&outputQueueManager,&inputQueueManager,devices,globalTime, database);
    this->informationDecodeTask = new pissInputInformationDecoder(&inputQueueManager,devices,datagrammeAnalyser);
    this->outputInformationEncoder = new pissOutputInformationEncoder();
    this->server = new pissServer(&inputQueueManager,&outputQueueManager,devices, datagrammeAnalyser,globalTime, database);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::connectBackRequest
//! \param ip
//! \param port
//!
void pissCommunicationStack::connectBackRequest(QString ip, int port){
    qDebug()<<"connectBackRequest"<<ip<<port;
    connectBack(false, ip, port);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::setDatabase
//! \param database
//!
void pissCommunicationStack::setDatabase(SystemDataBase* database){
    this->database = database;
    this->database->setDevices(this->devices);
    this->connect(this->devices, SIGNAL(update()), database, SLOT(updateDevices()));
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::clearBuffer
//!
void pissCommunicationStack::clearBuffer(){
    inputQueueManager.clear();
    outputQueueManager.clear();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::closeServer
//! \return
//!
bool pissCommunicationStack::closeServer(){
    informationDecodeTask->stop();
    //outputInformationEncoder->stop();
    return server->stopServer();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::launch
//!
bool pissCommunicationStack::launchServer(){
    informationDecodeTask->start();
    //outputInformationEncoder->start();
    return server->launchServer();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::connectBack
//! \param flag
//! \param addr
//! \param port
//! \return
//!
bool pissCommunicationStack::connectBack(bool flag, QString addr, int port){
    if(flag){
        //! motivate connect
        //! int id  = devices->addClient();
        igtClient *client = new igtClient(devices->getClientNumber(), &outputQueueManager, devices, globalTime);
        this->datagrammeAnalyser->setConnectBackRequestWaitingPair(devices->getClientNumber(), client->connectRequest(addr, port));

    }
    else{
        //! connect back process
        //! int id  = devices->addClient();
        igtClient *client = new igtClient(devices->getClientNumber()-1, &outputQueueManager, devices, globalTime);
        client->connectBackRequest(addr, port);
        //this->datagrammeAnalyser->setConnectBackRequestWaitingPair(devices->getClientNumber(), client->connectRequest(addr, port));
    }

    return true;
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::getNetworkEnvironment
//! \return
//!
Device* pissCommunicationStack::getLocalDevice(){
    return this->devices->getLocalDevice();
}
