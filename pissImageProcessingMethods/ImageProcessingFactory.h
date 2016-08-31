#ifndef IMAGEPROCESSINGFACTORY_H
#define IMAGEPROCESSINGFACTORY_H
#include "VesselEnhancementFilter.h"
#include "ErrorMessage.h"
#include <GeneralInformationAnalyser.h>
#include <vtkImageData.h>
#include <QVector>
#include <HistogramPoint.h>
#include "IgssVtkImageConverter.h"

class ImageProcessingFactory
{
public:
    ImageProcessingFactory();
    eProcessingErrorCode doProcessingByCommand(IgssImage *input, IgssImage *output, int dim, QString method);

    long getTotlaNumber(vtkImageData* mraImage);
    double getMean(vtkImageData* mraImage);
    QVector<HistogramPoint*> getHistogramFrom(vtkImageData* mraImage);
    QStringList getHistogramStatisticsFrom(vtkImageData* mraImage);

    GeneralInformationAnalyser* getGeneralInformationAnalyser();

    IgssVtkImageConverter *getIgssVtkImageConverter();

private:
    VesselEnhancementFilter *vesselEnhancementFilter;
    GeneralInformationAnalyser* generalInformationAnalyser;
    IgssVtkImageConverter *igssVtkImageConverter;


};

#endif // IMAGEPROCESSINGFACTORY_H
