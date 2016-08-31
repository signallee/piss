#include "ImageProcessingFactory.h"

ImageProcessingFactory::ImageProcessingFactory()
{
    this->vesselEnhancementFilter = new VesselEnhancementFilter();
    this->generalInformationAnalyser = new GeneralInformationAnalyser();
    this->igssVtkImageConverter = new IgssVtkImageConverter();
}

//!
//! \brief ImageProcessingFactory::doProcessingByCommand
//! \param input
//! \param output
//! \param dim
//! \param method
//! \return
//!
eProcessingErrorCode ImageProcessingFactory::doProcessingByCommand(IgssImage *input, IgssImage *output, int dim, QString method){
    eProcessingErrorCode ret = PROCESSING_NO_ERRROR;
    if(dim == 3){
        if(method == "vef"){
            ret = this->vesselEnhancementFilter->do_3D_VesselEnhancement(input,output);
        }
    }
    return ret;
}

//!
//! \brief ImageProcessingFactory::getIgssVtkImageConverter
//! \return
//!
IgssVtkImageConverter *ImageProcessingFactory::getIgssVtkImageConverter(){
    return this->igssVtkImageConverter;
}

//!
//! \brief ImageProcessingFactory::getGeneralInformationAnalyser
//! \return
//!
GeneralInformationAnalyser* ImageProcessingFactory::getGeneralInformationAnalyser(){
    return this->generalInformationAnalyser;
}

//!
//! \brief ImageProcessingFactory::getTotlaNumber
//! \param mraImage
//! \return
//!
long ImageProcessingFactory::getTotlaNumber(vtkImageData* mraImage){
    return this->generalInformationAnalyser->getTotalGrayscaleValueNumber(mraImage);
}

//!
//! \brief ImageProcessingFactory::getMean
//! \param mraImage
//! \return
//!
double ImageProcessingFactory::getMean(vtkImageData* mraImage){
    return this->generalInformationAnalyser->getGrayscaleValueMean(mraImage);
}

//!
//! \brief ImageProcessingFactory::getHistogramFrom
//! \param mraImage
//! \return
//!
QVector<HistogramPoint*> ImageProcessingFactory::getHistogramFrom(vtkImageData* mraImage){
    return this->getGeneralInformationAnalyser()->extractFrequency(mraImage);
}

//!
//! \brief ImageProcessingFactory::getHistogramStatisticsFrom
//! \param mraImage
//! \return
//!
QStringList ImageProcessingFactory::getHistogramStatisticsFrom(vtkImageData* mraImage){
    return this->getGeneralInformationAnalyser()->extractHistogramStatistics(mraImage);
}
