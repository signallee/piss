#include "SystemDataBase.h"


/**
 * @brief SystemDataBase::SystemDataBase
 */
SystemDataBase::SystemDataBase(){
    this->patients = new Patients();
    this->metaData = new SystemMetaData();
    this->carm = new CArm();

}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void SystemDataBase::setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
    this->patients->setImageProcessingFactory(imageProcessingFactory);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientsStatus
//! \return
//!
QStringList SystemDataBase::getPatientsStatus(){
    return this->patients->getPatientsStatus();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientMRAImageById
//! \param id
//! \return
//!
IgssImage* SystemDataBase::getPatientMRAImageById(int id){
    return this->patients->getPatientMRAImageById(id);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::setPaths
//! \param workspace_path
//! \param patients_path
//! \param metadata_path
//! \param image_center_path
//!
void SystemDataBase::setPaths(QString workspace_path,
                              QString patients_path,
                              QString metadata_path,
                              QString image_center_path){

    this->metaData->setSystemPaths(workspace_path,  patients_path,  metadata_path, image_center_path);
    this->patients->setPatientsPath(patients_path);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientByPesonnelInfo
//! \param name
//! \param birth
//! \return
//!
Patient* SystemDataBase::getPatientById(int id){
    return this->patients->getPatientById(id);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getSystemWorkSpace
//! \return
//!
QString SystemDataBase::getSystemWorkSpace(){
    return this->metaData->getSystemWorkSpace();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientsPath
//! \return
//!
QString SystemDataBase::getPatientsPath(){
    return this->metaData->getPatientsPath();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getMetaData
//! \return
//!
QString SystemDataBase::getMetaData(){
    return this->metaData->getMetaData();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getImageCenterPath
//! \return
//!
QString SystemDataBase::getImageCenterPath(){
    return this->metaData->getImageCenterPath();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::findPatientsExisted
//!
int SystemDataBase::findPatientsExisted(){
    return this->patients->findPatientExisted();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientsData
//! \return
//!
Patients* SystemDataBase::getPatientsData(){
    return this->patients;
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::setDevices
//! \param _device
//!
void SystemDataBase::setDevices(Devices *_device){
    this->devices = _device;
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::updateDevices
//!
void SystemDataBase::updateDevices(){
    qDebug()<<"updateDevices";
}
