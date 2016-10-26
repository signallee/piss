#ifndef PATIENT_H
#define PATIENT_H
#include <QList>
#include <IgssImage.h>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QImage>
#include <QString>
#include <QColor>
#include <QMap>
#include <QVector>
#include <QStringList>
#include "MRAFileReader.h"
#include "vtkImageData.h"
#include "vtkPoints.h"
#include <vtkSmartPointer.h>
#include "IgssVtkImageConverter.h"
#include "ImageProcessingFactory.h"
#include "ImageFileInterface.h"
#include <HistogramPoint.h>
#include <ColorPoint.h>
#include <TransferPoint.h>
#include "BidimensionnelImage.h"
#include "CenterLineReader.h"
#include "CenterLinePoint.h"


class Patient
{
private:
    class ThreadImageProcessing : public QThread{
        public:
            void run();
            Patient *patient;
    };

public:
    Patient(QString myPath, int id);

    bool doReadReconstructedResult();

    int getIdNumber();
    bool readFinished();
    QString getBirthdayOfPatient();
    void doImageFileLecture();
    QString getCTImagePath();
    QString getName();

    QString getFirstName();
    QString getLastName();

    QString getSex();
    QString getPhotoPath();
    QString getMraTridimensionelPath();
    QString getTridimensionelPath();
    IgssImage *getOriginImage();
    vtkImageData *getMraImageToBeDisplayed();
    void setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory);
    bool loadMRAImageFile(const QString &fileName);

    bool loadCenterLineFiles(QString fileName);

    void setMRAImage(vtkImageData *personnelMRAImage);

    QStringList getMriStatisticsList();
    QVector<HistogramPoint*> getMriHistogramfrequencies();
    QVector<HistogramPoint*> getGrayScaleValuefrequencies();
    QVector<TransferPoint*> getOpacityTransferPoints();
    QVector<ColorPoint*> getColorTransferPoints();
    QVector<TransferPoint*> getGradientTransferPoints();

    int getColorTransferPointsNumber();

    int findPointInTolerentArea(double abscissa, double ordinate, QString transformationFormat);

    void setGrayScaleValueFrequencies(QVector<HistogramPoint*> frequencies);
    void setOpacityTransferPoint(TransferPoint* opacityPoint);
    void setColorTransferPoint(ColorPoint* redPoint);
    void setGradientTransferPoint(TransferPoint* gradientPoint);

    int getMaximumGrayscaleValue();
    int getMinimumGrayscaleValue();

    int getGrayScaleValueByIndex(int index, QString option);
    double getOpacityValueByIndex(int index);
    double getGradientValueByIndex(int index);
    double getRedValueByIndex(int index);
    double getGreenValueByIndex(int index);
    double getBlueValueByIndex(int index);

    void updateOpacityPoint(int index, int abscissa, double ordinate);
    void updateGradientPoint(int index, int abscissa, double ordinate);
    void updateColorPoint(int index, int abscissa);

    void pushCTImage(BidimensionnelImage *img);
    QImage fetchLatestCTImageForDisplay();
    int getCTImageSequenceLength();

    QImage vtkImageDataToQImage(vtkImageData* imageData);

    vtkPoints *getCenterLinePoints();//! to be deleted

    vtkPoints*getCenterlineByName(QString name);

    QVector<CenterLinePoint*> getVesselByName(QString name);

    int getCenterLinePointsCount();

    QString getCenterLineFolderPath();
    void loadVesselByPath(QString path);

private:
    CenterLineReader *centerReader;
    vtkPoints *centerLinePoints;//! to be deleted

    QMap<QString, vtkPoints*> vesselCenterlines;
    QMap<QString, QVector<CenterLinePoint*>> vessels;

    int centerLinePointsCount;

    QVector<HistogramPoint*> frequencies;

    QVector<TransferPoint*> opacityTransferPoints;
    QVector<ColorPoint*> colorTransferPoints;
    QVector<TransferPoint*> gradientTransferPoints;

    QStringList statisticsList;

    int maximumGrayScaleValue;
    int minimumGrayScaleValue;

    bool MraImageReadComplete;

    QString myPath;
    QString myPhotoPath;
    QString mraTridimensionelPath;
    QString centerLineFolderPath;

    //! personnel informations
    QString name;
    QString birthday;
    QString firstName;
    QString lastName;
    QString sex;
    QString IdentityNumber;
    //QString sugeryDate;
    int imageExtents[6];

    int age;
    int id;

    ImageProcessingFactory* imageProcessingFactory;

    //! tridimensionnel Image (MRA)
    IgssImage *originImage;
    vtkImageData* MriImageForDisplay;

    //! bidimensionnel Image (CT) captured while operation
    QList<BidimensionnelImage *> cImageSequence;
    QList<QImage> cImageSequenceForDisplay;

    //! parameter of image......

    //! Handle du thread
    ThreadImageProcessing imageProcessingThread;
    MRAFileReader *mhdFileReader;

};

#endif // PATIENT_H
