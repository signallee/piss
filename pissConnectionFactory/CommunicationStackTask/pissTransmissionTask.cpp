#include "pissTransmissionTask.h"


pissTransmissionTask::pissTransmissionTask(int id, QVector<OutputQueue *> *oq, Devices* environment, QTcpSocket* socketTransmission){
    this->id = id;
    this->oq = oq;
    this->devices = environment;
    this->transmissionSocket = socketTransmission;

    frameCounter = 0;
    transmissionTimer = new QTimer();

    this->connect(this->transmissionTimer, SIGNAL(timeout()), this, SLOT(transfer()));
}

pissTransmissionTask::~pissTransmissionTask(){

}

void pissTransmissionTask::launch(){
    //this->transmissionTimer->start(20);
    this->start();
}

void pissTransmissionTask::run(){
    while(1){
        outputMutex.lock();
        if(oq->size() > 0){
            qDebug()<<"start transfer"<<id<<oq<<oq->at(0);
            if(oq->at(id)->getLength() > 0)
            {
                qDebug()<<oq->at(id)->fetchFirstDatagramme()->getValue();
                this->transmissionSocket->write(*(oq->at(id)->fetchFirstDatagramme()->getValue()));
                qDebug()<<"....";
                oq->at(id)->deleteFrontElement();
                frameCounter ++;
            }
            else{
                qDebug()<<"no msg in the outputqueue"<<id;
            }
        }
        outputMutex.unlock();
        sleep(1);
    }
}

void pissTransmissionTask::transfer(){
    qDebug()<<"start transfer";
//    outputMutex.lock();
//    if(oq->size() > 0){
//        int length = oq->at(id)->getLength();
//        if(length > 0)
//        {
//            this->transmissionSocket->write(*(oq->at(id)->fetchFirstDatagramme()->getValue()));
//            oq->at(id)->deleteFrontElement();
//            this->transmissionSocket->waitForBytesWritten(-1);
//            frameCounter ++;
//        }
//    }
//    outputMutex.unlock();
}
