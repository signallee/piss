#include "SystemDispatcher.h"

SystemDispatcher::SystemDispatcher(){
    this->initializeSystemPaths();
}

QStringList SystemDispatcher::getPatientsStatus(){
    return this->database->getPatientsStatus();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::extractBrainCortextFrom
//! \param input
//! \param output
//!
bool SystemDispatcher::extractBrainCortextFrom(vtkImageData *input, QString path){
    return this->imageProcessingFactory->extractBrainCortextFrom(input, path);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getHistogramOfVolumeData
//! \param img
//! \param points
//!
QVector<HistogramPoint*> SystemDispatcher::getHistogramOfVolumeData(vtkImageData *img){
   return this->imageProcessingFactory->getHistogramFrom(img);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getPatientInDataBase
//! \param name
//! \param birth
//! \return
//!
Patient* SystemDispatcher::getPatientById(int id){
    return this->database->getPatientById(id);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::startPissServer
//!
bool SystemDispatcher::launchCommunicationStackServer(){
    return communicationStack->launchServer();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::launchConnectBackProcess
//! \param flag
//! \param addr
//! \param port
//!
void SystemDispatcher::launchConnectBackProcess(bool flag, QString addr,int port){
    this->communicationStack->connectBack(flag, addr, port);
}


//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::stopPissServer
//! \return
//!
bool SystemDispatcher::stopPissServer(){
    return communicationStack->closeServer();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setMessageDisplayWindow
//! \param msgDisplay
//!
void SystemDispatcher::setMessageDisplayWindow(QTextEdit *msgDisplay){

}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief get_username
//! \return
//!
QString SystemDispatcher::get_username(){
    QString name = qgetenv("USER");
        if (name.isEmpty())
            name = qgetenv("USERNAME");
    return name;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::initializeSystemPaths
//!
void SystemDispatcher::initializeSystemPaths(){

    //! ------create some folders by predefined paths as the workspaces of the application-------------------
    QString username = get_username();

    if(username != "unknown") {
        //! define the software's deault path according to the os type

        #ifdef linux
        workspace_path = "/home/" + username + "/Documents/CanalyserWorkspace/";
        patients_path = workspace_path + "PatientsDataware/";
        metadata_path = workspace_path + "CanalyserMetadata/";
        image_center_path = workspace_path + "CanalyserImageCenter/";
        #elif _WIN64
        workspace_path = "C:\\Users\\" + username + "\\Documents\\CanalyserWorkspace\\";
        patients_path = workspace_path + "PatientsDataware\\";
        metadata_path = workspace_path + "CanalyserMetadata\\";
        image_center_path = workspace_path + "CanalyserImageCenter\\";
        #elif __APPLE__
        workspace_path = "/Users/" + username + "/Documents/CanalyserWorkspace/";
        patients_path = workspace_path + "PatientsDataware/";
        metadata_path = workspace_path + "CanalyserMetadata/";
        image_center_path = workspace_path + "CanalyserImageCenter/";
        #endif
    }
    else{
        //qDebug()<<"exception";
        return;
    }

    QFileInfo folder(workspace_path);

    if(!folder.isDir()){
        QDir path;
        path.mkdir(workspace_path);
        path.mkdir(patients_path);
        path.mkdir(metadata_path);
        path.mkdir(image_center_path);
    }
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setSystemDataBase
//! \param database
//!
void SystemDispatcher::setSystemDataBase(SystemDataBase* database){
    this->database = database;
    this->database->setPaths(this->workspace_path,
                             this->patients_path,
                             this->metadata_path,
                             this->image_center_path);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void SystemDispatcher::setImageProcessingFactory(ImageProcessingFactory *imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setCommunicationStack
//! \param communicationStack
//!
void SystemDispatcher::setCommunicationStack(pissCommunicationStack *communicationStack){
    this->communicationStack = communicationStack;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getImageCenterPath
//! \return
//!
QString SystemDispatcher::getImageCenterPath(){
     return this->image_center_path;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::findPatientExisted
//!
int SystemDispatcher::findPatientExisted(){
    return this->database->findPatientsExisted();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::doImageProcessingByMethodType
//! \param id
//! \param dim
//! \param method
//! \return
//!
QString SystemDispatcher::doImageProcessingByMethodType(int id, int dim, QString method){
    QString msg;
    output = new IgssImage();

    IgssImage*input = this->database->getPatientMRAImageById(id);

    eProcessingErrorCode ret = this->imageProcessingFactory->doProcessingByCommand(input,output,dim, method);

    if(ret == PROCESSING_NO_ERRROR){
        msg= method + " proceed with no error";
    }

    return msg;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getModuleConfig
//! \return
//!
Device* SystemDispatcher::getMyDeviceInfo(){
    return this->communicationStack->getLocalDevice();
}
