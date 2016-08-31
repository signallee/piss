#include "pissInputInformationDecoder.h"


/**
 * @brief pissInputInformationDecoder::pissInputInformationDecoder
 * @param inputQueueManager
 * @param NetworkEnvironment
 * @param datagrammeAnalyser
 */
pissInputInformationDecoder::pissInputInformationDecoder(QVector<InputQueue *> *inputQueueManager,
                                                         Devices *NetworkEnvironment,
                                                         DatagrammeAnalyser *datagrammeAnalyser)
{
    this->inputQueueManager = inputQueueManager;
    this->networkEnvironment = NetworkEnvironment;
    this->datagrammeAnalyser = datagrammeAnalyser;
    this->flag = true;
}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief pissInputInformationDecoder::~pissInputInformationDecoder
//!
pissInputInformationDecoder::~pissInputInformationDecoder(){

}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief pissInputInformationDecoder::stop
//!
void pissInputInformationDecoder::stop(){
    this->flag = false;
}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief pissInputInformationDecoder::run
//!
void pissInputInformationDecoder::run()
{
    while (this->flag){
        inputMutex.lock();
        if(inputQueueManager->size()>0){
            for(unsigned char cpt = 0; cpt < inputQueueManager->size(); cpt++){
                int length = inputQueueManager->at(cpt)->getLength();
                if(length > 0){
                    qDebug()<<"new message decoding";
                    datagrammeAnalyser->decodage(cpt, inputQueueManager->at(cpt)->fetchFirstDatagramme());
                    inputQueueManager->at(cpt)->deleteFirstDatagramme();
                }
                else{
                    //qDebug()<<"no message pushed into inputqueue at"<<cpt;
                }
            }
        }
        else{
            //qDebug()<<"no input queue generated";
        }

        inputMutex.unlock();
        usleep(2000);
        //qDebug()<<"pissInputInformationDecoder";
    }
}
