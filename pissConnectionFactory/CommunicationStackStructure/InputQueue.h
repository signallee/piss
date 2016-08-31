#ifndef INPUTQUEUE_H
#define INPUTQUEUE_H

#include<QVector>
#include"CDatagramme.h"

class InputQueue
{
public:
    InputQueue();
    ~InputQueue();

    void append(CDatagramme* datagramme);
    void clear();
    void deleteFirstDatagramme();
    CDatagramme* fetchFirstDatagramme();
    int getLength();

private:
    QVector <CDatagramme*> inputqueue;
};

#endif // INPUTQUEUE_H
