#ifndef OUTPUTQUEUE_H
#define OUTPUTQUEUE_H

#include <QList>
#include <QMutex>
#include "CDatagramme.h"


class OutputQueue
{
public:
    OutputQueue();
    ~OutputQueue();

    void append(CDatagramme* datagramme);
    void clear();
    void deleteFrontElement();
    CDatagramme* fetchFirstDatagramme();
    int getLength();
private:
    QList <CDatagramme*> outputqueue;
    QMutex oqMutex;
    long cpt;
};

#endif // OUTPUTQUEUE_H
