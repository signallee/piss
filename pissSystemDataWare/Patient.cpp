#include "Patient.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>


//!----------------------------------------------------------------------------
//!
//! \brief Patient::ThreadImageProcessing::run
//!
void Patient::ThreadImageProcessing::run(){

    //! check if patient's mraImage exist in the personnel mra path....
    QFileInfo checkMHDFile(patient->myPath + "/mra_tridimensionel__image/" + patient->name + ".mhd");
    QString mraImagefilePath;

    //! check if file exists and if yes: Is it really a file and no directory?
    if((checkMHDFile.exists() && checkMHDFile.isFile()) ){
        mraImagefilePath = patient->myPath + "/mra_tridimensionel__image/" + patient->name + ".mhd";
        patient->loadMRAImageFile(mraImagefilePath);

        patient->statisticsList = patient->imageProcessingFactory->getHistogramStatisticsFrom(patient->MriImageForDisplay);
        patient->maximumGrayScaleValue = patient->statisticsList[0].toInt(0, 10);
        patient->minimumGrayScaleValue = patient->statisticsList[1].toInt(0, 10);

        patient->frequencies = patient->imageProcessingFactory->getHistogramFrom(patient->MriImageForDisplay);
    }
    else{

    }
    //! center lines lecture
    QFileInfo checkCenterLineFile(patient->myPath + "/mra_tridimensionel__image/centerlines");

    QString centerlinefolderPath;
    if((checkCenterLineFile.exists() && checkCenterLineFile.isDir()) ){
        centerlinefolderPath = patient->myPath + "/mra_tridimensionel__image/centerlines";
        patient->loadCenterLineFiles(centerlinefolderPath);
    }
    patient->MraImageReadComplete = true;
}

//!----------------------------------------------------------------------------
//!
//! \brief Patient::Patient
//! \param myPath
//!
Patient::Patient(QString myPath, int id){
    this->id = id;
    this->myPath = myPath;

    MraImageReadComplete = false;

    //! fetch name and birth of the patient
    QStringList temp = myPath.split("/");
    QStringList patientInfo = temp[temp.size()-1].split("__");
    name = patientInfo[0];

    QStringList name_temp = name.split("_");
    this->firstName = name_temp[1];
    this->lastName = name_temp[0];

    birthday = patientInfo[1];
    myPhotoPath = myPath + "/personnelInfoPath/" + name +".png";
    mraTridimensionelPath = myPath + "/mra_tridimensionel__image/" + name + ".mhd";
    centerLineFolderPath = myPath + "/mra_tridimensionel__image/centerlines/";

    maximumGrayScaleValue = 0;
    minimumGrayScaleValue = 0;

    originImage = new IgssImage();

    centerReader = new CenterLineReader();

    centerLinePoints = vtkPoints::New();

    this->cImageSequence.clear();

}

//! ----------------------------------------------------------------------------
//!
//! \brief Patient::doReadReconstructedResult
//!
bool Patient::doReadReconstructedResult(){
    //qDebug()<<myPath + "/bidimensionel__images/reconstruct/result.raw";
    QFileInfo checkMHDFile(myPath + "/bidimensionel__images/reconstruct/result.raw");
    QString mraImagefilePath;

    //! check if file exists and if yes: Is it really a file and no directory?
    if((checkMHDFile.exists() && checkMHDFile.isFile()) ){

        mraImagefilePath = myPath + "/bidimensionel__images/reconstruct/result.mhd";
        loadMRAImageFile(mraImagefilePath);

        statisticsList =imageProcessingFactory->getHistogramStatisticsFrom(MriImageForDisplay);
        maximumGrayScaleValue = statisticsList[0].toInt(0, 10);
        minimumGrayScaleValue = statisticsList[1].toInt(0, 10);

        frequencies = imageProcessingFactory->getHistogramFrom(MriImageForDisplay);

        //QFile::remove(myPath + "/bidimensionel__images/reconstruct/result.raw");

        return true;
    }

    return false;
}

//! ----------------------------------------------------------------------------
//!
//! \brief Patient::getCenterLineFolderPath
//! \return
//!
QString Patient::getCenterLineFolderPath(){
    return this->centerLineFolderPath;
}

//! ----------------------------------------------------------------------------
//!
//! \brief Patient::getPoints
//! \return
//!
vtkPoints *Patient::getCenterLinePoints(){
    return this->centerLinePoints;
}

//! ----------------------------------------------------------------------------
//!
//! \brief Patient::getCenterLinePointsCount
//! \return
//!
int Patient::getCenterLinePointsCount(){
    return this->centerLinePointsCount;
}

//!----------------------------------------------------------------------------
//!
//! \brief Patient::vtkImageDataToQImage
//! \param imageData
//! \return
//!
QImage Patient::vtkImageDataToQImage(vtkImageData* imageData){
  if (!imageData){
    return QImage();
  }

  /// \todo retrieve just the UpdateExtent
  int width = imageData->GetDimensions()[0];
  int height = imageData->GetDimensions()[1];

  QImage image(width, height, QImage::Format_RGB32);

  QRgb* rgbPtr = reinterpret_cast<QRgb*>(image.bits()) + width * (height-1);

  unsigned char* colorsPtr = reinterpret_cast<unsigned char*>(imageData->GetScalarPointer());
  // mirror vertically

  for(int row = 0; row < height; ++row){
    for (int col = 0; col < width; ++col){
      // Swap rgb
      *(rgbPtr++) = QColor(colorsPtr[0], colorsPtr[1], colorsPtr[2]).rgb();
      colorsPtr +=  3;
    }
        rgbPtr -= width * 2;
    }
  return image;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::pushCTImage
//! \param img
//!
void Patient::pushCTImage(BidimensionnelImage *img){
    this->cImageSequence.append(img);
    this->cImageSequenceForDisplay.append(this->vtkImageDataToQImage(img->getImage()));
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::fetchCTImage
//! \return
//!
QImage Patient::fetchLatestCTImageForDisplay(){

    this->cImageSequence.takeLast();
    return this->cImageSequenceForDisplay.takeLast();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getCTImageSequenceLength
//! \return
//!
int Patient::getCTImageSequenceLength(){
    return this->cImageSequence.size();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGrayScaleValueByIndex
//! \return
//!
int Patient::getGrayScaleValueByIndex(int index, QString option){
    int grayScaleValue = 0;
     if(option == "opacity"){
         grayScaleValue =  this->opacityTransferPoints[index]->getAbscissaValue();
     }
     else if(option == "gradient"){
         grayScaleValue = this->gradientTransferPoints[index]->getAbscissaValue();
     }
     else{
         grayScaleValue = this->colorTransferPoints[index]->getAbscissaValue();
     }

     return grayScaleValue;

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::updateOpacityPoint
//! \param index
//! \param abscissa
//! \param ordinate
//!
void Patient::updateOpacityPoint(int index, int abscissa, double ordinate){
    this->opacityTransferPoints[index]->setAbscissaValue(abscissa);
    this->opacityTransferPoints[index]->setOrdinateValue(ordinate);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::updateGradientPoint
//! \param index
//! \param abscissa
//! \param ordinate
//!
void Patient::updateGradientPoint(int index, int abscissa, double ordinate){
    this->gradientTransferPoints[index]->setAbscissaValue(abscissa);
    this->gradientTransferPoints[index]->setOrdinateValue(ordinate);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::updateColorPoint
//! \param index
//! \param abscissa
//! \param ordinate
//!
void Patient::updateColorPoint(int index, int abscissa){
    this->colorTransferPoints[index]->setAbscissaValue(abscissa);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getOpacityValueByIndex
//! \return
//!
double Patient::getOpacityValueByIndex(int index){
    return this->opacityTransferPoints[index]->getOrdinateValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGradientValueByIndex
//! \param index
//! \return
//!
double Patient::getGradientValueByIndex(int index){
    return this->gradientTransferPoints[index]->getOrdinateValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getRedValueByIndex
//! \param index
//! \return
//!
double Patient::getRedValueByIndex(int index){
    return this->colorTransferPoints[index]->getRedValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGreenValueByIndex
//! \param index
//! \return
//!
double Patient::getGreenValueByIndex(int index){
    return this->colorTransferPoints[index]->getGreenValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getBlueValueByIndex
//! \param index
//! \return
//!
double Patient::getBlueValueByIndex(int index){
    return this->colorTransferPoints[index]->getBlueValue();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::findPointInTolerentArea
//! \param abscissa
//! \param ordinate
//! \param transformationFormat
//! \return
//!
int Patient::findPointInTolerentArea(double abscissa, double ordinate, QString transformationFormat){
    if(transformationFormat == "opacity"){
        for(unsigned char cpt = 0;  cpt < this->opacityTransferPoints.size(); cpt++){
            if(qAbs(abscissa - this->opacityTransferPoints[cpt]->getAbscissaValue())<15&&qAbs(ordinate - this->opacityTransferPoints[cpt]->getOrdinateValue())<0.1){
                return cpt;
            }
        }
    }
    else if(transformationFormat == "color"){
        for(unsigned char cpt = 0;  cpt < this->colorTransferPoints.size(); cpt++){
            if(qAbs(abscissa - this->colorTransferPoints[cpt]->getAbscissaValue())<10){
                return cpt;
            }
        }
    }
    else{
        for(unsigned char cpt = 0;  cpt < this->gradientTransferPoints.size(); cpt++){
            if(qAbs(abscissa - this->gradientTransferPoints[cpt]->getAbscissaValue())<15&&qAbs(ordinate - this->gradientTransferPoints[cpt]->getOrdinateValue())<0.1){
                return cpt;
            }

        }
    }
    return -1;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getOpacityTransferPoints
//! \return
//!
QVector<TransferPoint*> Patient::getOpacityTransferPoints(){
    return this->opacityTransferPoints;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getColorTransferPoints
//! \return
//!
QVector<ColorPoint*> Patient::getColorTransferPoints(){
    return this->colorTransferPoints;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getColorTransferPointsNumber
//! \return
//!
int Patient::getColorTransferPointsNumber(){
    return this->colorTransferPoints.size();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getGradientTransferPoints
//! \return
//!
QVector<TransferPoint*> Patient::getGradientTransferPoints(){
    return this->gradientTransferPoints;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setGrayScaleValueFrequencies
//! \param frequencies
//!
void Patient::setGrayScaleValueFrequencies(QVector<HistogramPoint*> frequencies){
    this->frequencies = frequencies;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setOpacityTransferPoint
//! \param opacityPoint
//!
void Patient::setOpacityTransferPoint(TransferPoint* opacityPoint){
    this->opacityTransferPoints.append(opacityPoint);

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setColorTransferPoint
//! \param redPoint
//!
void Patient::setColorTransferPoint(ColorPoint* redPoint){
    //TODO
    this->colorTransferPoints.append(redPoint);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setGradientTransferPoint
//! \param opacityPoint
//!
void Patient::setGradientTransferPoint(TransferPoint* gradientPoint){
    this->gradientTransferPoints.append(gradientPoint);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMaximumGrayscaleValue
//! \return
//!
int Patient::getMaximumGrayscaleValue(){
    return this->maximumGrayScaleValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMinimumGrayscaleValue
//! \return
//!
int Patient::getMinimumGrayscaleValue(){
    return this->minimumGrayScaleValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMriHistogramfrequencies
//! \return
//!
QVector<HistogramPoint*> Patient::getMriHistogramfrequencies(){
    return this->frequencies;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getMriStatisticsList
//! \return
//!
QStringList Patient::getMriStatisticsList(){
    return this->statisticsList;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getFirstName
//! \return
//!
QString Patient::getFirstName(){
    return this->firstName;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getLastName
//! \return
//!
QString Patient::getLastName(){
    return this->lastName;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//!
//! \brief Patient::getMraImageToBeDisplayed
//! \return
//!
vtkImageData *Patient::getMraImageToBeDisplayed(){
    return this->MriImageForDisplay;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getVesselByName
//! \param name
//! \return
//!
vtkPoints* Patient::getCenterlineByName(QString name){
    return vesselCenterlines[name];
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::getVesselByName
//! \param name
//! \return
//!
QVector<CenterLinePoint*> Patient::getVesselByName(QString name){
    return vessels[name];
}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadVesselByPath
//! \param path
//!
void Patient::loadVesselByPath(QString path){
    vtkPoints *centerline = vtkPoints::New();
    centerReader->doReadCenterLineFile(centerLineFolderPath+path, centerline);
    vesselCenterlines[path] = centerline;

    vessels[path] = centerReader->doReadComplteCenterlineFile(centerLineFolderPath+path);
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadCenterLineFiles
//! \param fileName
//! \return
//!
bool Patient::loadCenterLineFiles(QString fileName){
    CenterLineReader *centerReader = new CenterLineReader();
    centerReader->doReadCenterlineFolder(fileName);
    this->centerLinePointsCount = centerReader->get_vesselsPoints().size();
    for(int i = 0; i < this->centerLinePointsCount; i++){
        centerLinePoints->InsertPoint(i,
                            centerReader->get_vesselsPoints().at(i)->get_abscissa(),
                            centerReader->get_vesselsPoints().at(i)->get_ordinate(),
                            centerReader->get_vesselsPoints().at(i)->get_isometric());
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadMRAImageFile
//! \param fileName
//! \return
//!
bool Patient::loadMRAImageFile(const QString &fileName){
    //! file type check
    eFileType ret =  ImageFileInterface::getFileType(fileName);

    if(ret == UNKOWN_FILE_TYPE){
        return false;
    }

    //! get the instance of related reader according to the file type
    ImageFileInterface *fileInterface = ImageFileInterface::getInstanceFileByType(ret);

    //! read the content of the image
    if(fileInterface->readFrom(fileName) != IMAGE_NO_ERRROR){
        return false;
    }

    //!Pass the reference of the image read to the pointer declared upon
    this->setMRAImage(fileInterface->getImage());
    fileInterface->getImageExtentInformation(imageExtents);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::setMRAImage
//! \param personnelMRAImage
//!
void Patient::setMRAImage(vtkImageData *personnelMRAImage){
    this->MriImageForDisplay = personnelMRAImage;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void Patient::setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::readFinished
//! \return
//!
bool Patient::readFinished(){
    return this->MraImageReadComplete;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getIdNumber
//! \return
//!
int Patient::getIdNumber(){
    return this->id;
}


//!------------------------------------------------------------------------------
//!
//! \brief Patient::getBirthdayOfPatient
//! \return
//!
QString Patient::getBirthdayOfPatient(){
    return this->birthday;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::doImageProcessing
//!
void Patient::doImageFileLecture(){
    imageProcessingThread.patient = this;
    imageProcessingThread.start();
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getMRAPath
//! \return
//!
QString Patient::getCTImagePath(){
    return this->myPath + "/bidimensionel__images/";
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getPhotoPath
//! \return
//!
QString Patient::getPhotoPath(){
    return this->myPhotoPath;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getTridimensionelPath
//! \return
//!
QString Patient::getTridimensionelPath(){
    return this->myPath + "/mra_tridimensionel__image/";
}

//!------------------------------------------------------------------------------
//!
//! \brief getMraTridimensionelPath
//! \return
//!
QString Patient::getMraTridimensionelPath(){
    return this->mraTridimensionelPath;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getName
//! \return
//!
QString Patient::getName(){
    return this->name;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getSex
//! \return
//!
QString Patient::getSex(){
    return this->sex;
}

//!------------------------------------------------------------------------------
//!
//! \brief Patient::getOriginImage
//! \return
//!
IgssImage *Patient::getOriginImage(){
    return this->originImage;
}

