#include "OutputQueue.h"

OutputQueue::OutputQueue()
{
    cpt = 0;
    clear();
}

OutputQueue::~OutputQueue()
{

}

void OutputQueue::append(CDatagramme *datagramme)
{
    oqMutex.lock();
    outputqueue.append(datagramme);
    cpt ++;
    oqMutex.unlock();
}

void OutputQueue::clear(){
    oqMutex.lock();
    outputqueue.clear();
    cpt = 0;
    oqMutex.unlock();
}

void OutputQueue::deleteFrontElement()
{
    oqMutex.lock();
    outputqueue.pop_front();
    cpt --;
    oqMutex.unlock();
}

CDatagramme* OutputQueue::fetchFirstDatagramme()
{
    CDatagramme *ret;
    oqMutex.lock();
    ret = outputqueue.at(0);
    oqMutex.unlock();
    return ret;
}

int OutputQueue::getLength(){

    int ret = 0;
    oqMutex.lock();
    ret = cpt;
    oqMutex.unlock();
    return ret;
}
