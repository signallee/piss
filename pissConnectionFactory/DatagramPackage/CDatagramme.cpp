#include "CDatagramme.h"


/**
 * @brief CDatagramme::CDatagramme
 * @param value
 */
CDatagramme::CDatagramme(){

}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::~CDatagramme
//!
CDatagramme::~CDatagramme(){

}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::setValue
//! \param value
//!
void CDatagramme::setValue(QByteArray* v){
    qDebug()<<v;
    this->value = v;
    this->totalLength = this->value->length();
}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::getValue
//! \return
//!
QByteArray* CDatagramme::getValue(){
    return this->value;
}

void CDatagramme::printSelf(){
    qDebug()<<this->totalLength<<":"<<this->value;

}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::getIdentifierByte
//! \return
//!
unsigned char CDatagramme::getDataType(){
    qDebug()<<this->value;
    qDebug()<<this->value->size();

    return (unsigned char)this->value->at(0);
}

//!------------------------------------------------------------
//!
//! \brief CDatagramme::getDeviceId
//! \return
//!
unsigned char CDatagramme::getDeviceId(){
    return this->value->at(1);
}

//!------------------------------------------------------------
//!
//! \brief CDatagramme::setTimestamp
//! \param time
//!
void CDatagramme::setTimestamp(qint32 time){
//    (*this->value)[2] = time % int(pow(2, 8));
//    (*this->value)[3] = time / int(pow(2, 8)) % int(pow(2, 8));
//    (*this->value)[4] = time / int(pow(2, 16)) % int(pow(2,8));
//    (*this->value)[5] = time / int(pow(2, 24)) % int(pow(2,8));
}

//!------------------------------------------------------------
//!
//! \brief CDatagramme::getTimestamp
//! \return
//!
unsigned long long CDatagramme::getTimestamp()
{
    return     (unsigned char)(value->at(9))*quint64(pow(2, 56))
             + (unsigned char)(value->at(8))*quint64(pow(2, 48))
             + (unsigned char)(value->at(7))*quint64(pow(2, 40))
             + (unsigned char)(value->at(6))*quint64(pow(2, 32))
             + (unsigned char)(value->at(5))*quint64(pow(2, 24))
             + (unsigned char)(value->at(4))*quint64(pow(2, 16))
             + (unsigned char)(value->at(3))*quint64(pow(2, 8))
             + (unsigned char)(value->at(2));
}

//!------------------------------------------------------------
//!
//! \brief CDatagramme::getDLC
//! \return
//!
int CDatagramme::getDLC()
{
    return (unsigned char)(this->value->at(11))*256 + (unsigned char)(this->value->at(10));
}
