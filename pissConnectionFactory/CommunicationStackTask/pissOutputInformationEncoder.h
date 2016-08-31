#ifndef PISSOUTPUTINFORMATIONENCODER_H
#define PISSOUTPUTINFORMATIONENCODER_H

#include <QThread>


class pissOutputInformationEncoder : public QThread
{
public:
    pissOutputInformationEncoder();
    ~pissOutputInformationEncoder();
};

#endif // PISSOUTPUTINFORMATIONENCODER_H
