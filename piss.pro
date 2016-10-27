#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T10:03:34
#
#-------------------------------------------------

QT       += multimedia network core gui multimediawidgets
#CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

unix:QMAKE_CXXFLAGS += -Wno-deprecated

TARGET = piss
TEMPLATE = app

INCLUDEPATH += pissSystemDataWare

INCLUDEPATH += pissIHM

INCLUDEPATH += pissConnectionFactory

INCLUDEPATH += pissConnectionFactory/DatagramPackage

INCLUDEPATH += pissConnectionFactory/CommunicationStackStructure

INCLUDEPATH += pissConnectionFactory/CommunicationStackTask

INCLUDEPATH += pissIHM/CurveReformationWindow

INCLUDEPATH += pissIHM/GuiComponentLib

INCLUDEPATH += pissIHM/GuidewareTrackingWindow

INCLUDEPATH += pissIHM/SurgeryPlanWindow

INCLUDEPATH += pissIHM/pissMainWindow

INCLUDEPATH += pissIHM/SuperviseWindow

INCLUDEPATH += pissIHM/ControlConsoleWindow

INCLUDEPATH += pissDispatcher

INCLUDEPATH += pissDiskImageReaderFactory

INCLUDEPATH += pissImageProcessingMethods

INCLUDEPATH += pissImageProcessingMethods/igssGeneralLib

INCLUDEPATH += pissImageProcessingMethods/igssMathLib

INCLUDEPATH += pissImageProcessingMethods/igssSegmentationLib

INCLUDEPATH += pissImageProcessingMethods/igssCurvePlanReformationLib

RESOURCES = imageresources.qrc

SOURCES +=  main.cpp\
            pissDispatcher/SystemDispatcher.cpp \
            pissSystemDataWare/SystemDataBase.cpp \
            pissSystemDataWare/igssImage.cpp \
            pissImageProcessingMethods/igssGeneralLib/igssVtkImageConverter.cpp \
            pissSystemDataWare/CenterLinePoint.cpp \
            pissSystemDataWare/Patients.cpp \
            pissSystemDataWare/SystemMetaData.cpp \
            pissSystemDataWare/Patient.cpp \
            pissImageProcessingMethods/igssMathLib/Derivation.cpp \
            pissImageProcessingMethods/igssMathLib/Convolution.cpp \
            pissImageProcessingMethods/igssSegmentationLib/VesselEnhancementFilter.cpp \
            pissImageProcessingMethods/igssSegmentationLib/Options.cpp \
            pissDiskImageReaderFactory/MRAFileReader.cpp \
            pissIHM/pissMainWindow/PatientsWidget.cpp \
            pissIHM/pissMainWindow/SurgerySystemWidget.cpp \
            pissIHM/pissMainWindow/ReplaysWidget.cpp \
            pissImageProcessingMethods/ImageProcessingFactory.cpp \
            pissImageProcessingMethods/ErrorMessage.cpp \
            pissIHM/pissMainWindow/AlgorithmTestPlatform.cpp \
            pissDiskImageReaderFactory/DicomCDRomReader.cpp \
            pissIHM/SurgeryPlanWindow/SurgeryPlanWindow.cpp \
            pissIHM/GuiComponentLib/CPushButton.cpp \
            pissIHM/GuiComponentLib/PlottingBoard.cpp \
            pissIHM/GuiComponentLib/ColorPoint.cpp \
            pissIHM/GuiComponentLib/HistogramPoint.cpp \
            pissIHM/GuiComponentLib/PlotPoint.cpp \
            pissIHM/GuiComponentLib/qcustomplot.cpp \
            pissIHM/GuiComponentLib/TransferPoint.cpp \
            pissIHM/SurgeryPlanWindow/PatientWidgetConfigurationBoard.cpp \
            pissIHM/GuiComponentLib/ColorChooseWindow.cpp \
            pissIHM/CurveReformationWindow/CurveReformationWindow.cpp \
            pissIHM/GuidewareTrackingWindow/GuidewareTrackingWindow.cpp \
            pissSystemDataWare/CTBidimensionnelFrame.cpp \
            pissIHM/pissMainWindow/PatientInformationWidget.cpp \
            pissIHM/pissMainWindow/PersonnelImageLabel.cpp \
            pissImageProcessingMethods/igssGeneralLib/GeneralInformationAnalyser.cpp \
            pissDiskImageReaderFactory/ImageFileInterface.cpp \
            pissDiskImageReaderFactory/ImageFileInterface_Dicomdir.cpp \
            pissDiskImageReaderFactory/ImageFileInterface_RawMhd.cpp \
            pissIHM/pissMainWindow/PatientItem.cpp \
            pissSystemDataWare/BidimensionnelImage.cpp \
            pissIHM/pissMainWindow/pissMainWindow.cpp \
            pissIHM/SuperviseWindow/SuperviseWindow.cpp \
            pissDiskImageReaderFactory/ImageFileInterface_Mat.cpp \
            pissDiskImageReaderFactory/CenterLineReader.cpp \
            pissIHM/ControlConsoleWindow/ControlConsoleWindow.cpp \
            pissIHM/pissMainWindow/SurgeryLoadingWindow.cpp \
            pissConnectionFactory/CommunicationStackTask/pissServer.cpp \
            pissConnectionFactory/CommunicationStackStructure/InputQueue.cpp \
            pissConnectionFactory/CommunicationStackStructure/OutputQueue.cpp \
            pissImageProcessingMethods/igssMathLib/vtkFrenetSerretFrame.cxx \
            pissImageProcessingMethods/igssMathLib/vtkSplineDrivenImageSlicer.cxx \
            pissConnectionFactory/CommunicationStackTask/pissReceptionTask.cpp \
            pissConnectionFactory/CommunicationStackTask/pissTransmissionTask.cpp \
            pissConnectionFactory/CommunicationStackTask/pissInputInformationDecoder.cpp \
            pissConnectionFactory/CommunicationStackTask/pissOutputInformationEncoder.cpp \
            pissConnectionFactory/GlobalTime.cpp \
            pissConnectionFactory/pissCommunicationStack.cpp \
            pissSystemDataWare/CArm.cpp \
            pissSystemDataWare/InterventionalTool.cpp \
            pissIHM/pissMainWindow/SystemOptions.cpp \
            pissIHM/pissMainWindow/SystemComponentsContainer.cpp \
            pissIHM/pissMainWindow/CommunicationStatesContainer.cpp \
            pissConnectionFactory/CommunicationStackStructure/Device.cpp \
            pissConnectionFactory/CommunicationStackStructure/Devices.cpp \
            pissConnectionFactory/CommunicationStackTask/igtClient.cpp \
            pissSystemDataWare/AcquisitionPoint.cpp \
            pissConnectionFactory/DatagramPackage/CDatagramme.cpp \
            pissConnectionFactory/DatagramPackage/DatagrammeAnalyser.cpp \
            pissDiskImageReaderFactory/MatFileReader.cpp \
            pissImageProcessingMethods/igssGeneralLib/BidimentionVTKConverter.cpp \
            pissConnectionFactory/DatagramPackage/HandShakeCommitMessage.cpp \
            pissConnectionFactory/DatagramPackage/HelloMessage.cpp \
            pissImageProcessingMethods/igssCurvePlanReformationLib/igsssCutter.cpp \
            pissConnectionFactory/DatagramPackage/HandShakeMessage.cpp \
    pissIHM/pissMainWindow/vtkResliceCursorCallback.cpp \
    pissImageProcessingMethods/igssGeneralLib/SeedPoint.cpp

HEADERS  += \
            pissDispatcher/SystemDispatcher.h \
            pissSystemDataWare/SystemDataBase.h \
            pissSystemDataWare/igssImage.h \
            pissImageProcessingMethods/igssGeneralLib/igssVtkImageConverter.h \
            pissSystemDataWare/CenterLinePoint.h \
            pissSystemDataWare/Patients.h \
            pissSystemDataWare/SystemMetaData.h \
            pissSystemDataWare/Patient.h \
            pissImageProcessingMethods/igssMathLib/Derivation.h \
            pissImageProcessingMethods/igssMathLib/Convolution.h \
            pissImageProcessingMethods/igssSegmentationLib/VesselEnhancementFilter.h \
            pissImageProcessingMethods/igssSegmentationLib/Options.h \
            pissDiskImageReaderFactory/MRAFileReader.h \
            pissIHM/pissMainWindow/PatientsWidget.h \
            pissIHM/pissMainWindow/SurgerySystemWidget.h \
            pissIHM/pissMainWindow/ReplaysWidget.h \
            pissImageProcessingMethods/ImageProcessingFactory.h \
            pissImageProcessingMethods/ErrorMessage.h \
            pissIHM/pissMainWindow/AlgorithmTestPlatform.h \
            pissDiskImageReaderFactory/DicomCDRomReader.h \
            pissIHM/SurgeryPlanWindow/SurgeryPlanWindow.h \
            pissIHM/GuiComponentLib/CPushButton.h \
            pissIHM/GuiComponentLib/PlottingBoard.h \
            pissIHM/GuiComponentLib/ColorPoint.h \
            pissIHM/GuiComponentLib/HistogramPoint.h \
            pissIHM/GuiComponentLib/PlotPoint.h \
            pissIHM/GuiComponentLib/qcustomplot.h \
            pissIHM/GuiComponentLib/TransferPoint.h \
            pissIHM/SurgeryPlanWindow/PatientWidgetConfigurationBoard.h \
            pissIHM/GuiComponentLib/ColorChooseWindow.h \
            pissIHM/CurveReformationWindow/CurveReformationWindow.h \
            pissIHM/GuidewareTrackingWindow/GuidewareTrackingWindow.h \
            pissIHM/pissMainWindow/PatientInformationWidget.h\
            pissSystemDataWare/CTBidimensionnelFrame.h \
            pissIHM/pissMainWindow/PersonnelImageLabel.h \
            pissImageProcessingMethods/igssGeneralLib/GeneralInformationAnalyser.h \
            pissDiskImageReaderFactory/ImageFileInterface.h \
            pissDiskImageReaderFactory/ImageFileInterface_Dicomdir.h \
            pissDiskImageReaderFactory/ImageFileInterface_RawMhd.h \
            pissIHM/SurgeryPlanWindow/digitalSurgeryTime.h \
            pissIHM/pissMainWindow/PatientItem.h \
            pissSystemDataWare/BidimensionnelImage.h \
            pissIHM/pissMainWindow/pissMainWindow.h \
            pissIHM/SuperviseWindow/SuperviseWindow.h \
            pissDiskImageReaderFactory/ImageFileInterface_Mat.h \
            pissDiskImageReaderFactory/CenterLineReader.h \
            pissIHM/ControlConsoleWindow/ControlConsoleWindow.h \
            pissIHM/pissMainWindow/SurgeryLoadingWindow.h \
            pissConnectionFactory/CommunicationStackTask/pissServer.h \
            pissConnectionFactory/CommunicationStackStructure/InputQueue.h \
            pissConnectionFactory/CommunicationStackStructure/OutputQueue.h \
            pissImageProcessingMethods/igssMathLib/vtkFrenetSerretFrame.h \
            pissImageProcessingMethods/igssMathLib/vtkSplineDrivenImageSlicer.h \
            pissConnectionFactory/CommunicationStackTask/pissReceptionTask.h \
            pissConnectionFactory/CommunicationStackTask/pissTransmissionTask.h \
            pissConnectionFactory/CommunicationStackTask/pissInputInformationDecoder.h \
            pissConnectionFactory/CommunicationStackTask/pissOutputInformationEncoder.h \
            pissConnectionFactory/GlobalTime.h \
            pissConnectionFactory/pissCommunicationStack.h \
            pissSystemDataWare/CArm.h \
            pissSystemDataWare/InterventionalTool.h \
            pissIHM/pissMainWindow/SystemOptions.h \
            pissIHM/pissMainWindow/SystemComponentsContainer.h \
            pissIHM/pissMainWindow/CommunicationStatesContainer.h \
            pissConnectionFactory/CommunicationStackStructure/Device.h \
            pissConnectionFactory/CommunicationStackStructure/Devices.h \
            pissConnectionFactory/CommunicationStackTask/igtClient.h \
            pissSystemDataWare/AcquisitionPoint.h \
            pissConnectionFactory/DatagramPackage/CDatagramme.h \
            pissConnectionFactory/DatagramPackage/DatagrammeAnalyser.h \
            pissDiskImageReaderFactory/MatFileReader.h \
            pissImageProcessingMethods/igssGeneralLib/BidimentionVTKConverter.h \
            pissConnectionFactory/DatagramPackage/HandShakeCommitMessage.h \
            pissConnectionFactory/DatagramPackage/HelloMessage.h \
            pissImageProcessingMethods/igssCurvePlanReformationLib/igsssCutter.h \
            pissConnectionFactory/DatagramPackage/HandShakeMessage.h \
    pissIHM/pissMainWindow/vtkResliceCursorCallback.h \
    pissImageProcessingMethods/igssGeneralLib/SeedPoint.h



win32:LIBS += $$quote(C:\Program Files\VTK\lib\QVTKWidgetPlugin.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkalglib-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkChartsCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonColor-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonComputationalGeometry-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonDataModel-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonExecutionModel-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonMath-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonMisc-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonSystem-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkCommonTransforms-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkDICOMParser-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkDomainsChemistry-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkexoIIc-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkexpat-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersAMR-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersExtraction-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersFlowPaths-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersGeneral-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersGeneric-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersGeometry-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersHybrid-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersHyperTree-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersImaging-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersModeling-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersParallel-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersParallelImaging-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersProgrammable-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersSelection-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersSMP-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersSources-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersStatistics-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersTexture-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkFiltersVerdict-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkfreetype-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkGeovisCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkgl2ps-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkGUISupportQt-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkGUISupportQtOpenGL-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkGUISupportQtSQL-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkhdf5-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkhdf5_hl-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingColor-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingFourier-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingGeneral-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingHybrid-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingMath-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingMorphological-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingSources-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingStatistics-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkImagingStencil-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkInfovisCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkInfovisLayout-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkInteractionImage-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkInteractionStyle-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkInteractionWidgets-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOAMR-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOEnSight-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOExodus-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOExport-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOGeometry-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOImage-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOImport-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOInfovis-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOLegacy-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOLSDyna-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOMINC-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOMovie-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIONetCDF-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOParallel-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOParallelXML-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOPLY-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOSQL-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOVideo-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOXML-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkIOXMLParser-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkjpeg-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkjsoncpp-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtklibxml2-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkmetaio-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkNetCDF-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkNetCDF_cxx-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkoggtheora-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkParallelCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkpng-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkproj4-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingAnnotation-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingContext2D-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingContextOpenGL-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingFreeType-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingGL2PS-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingImage-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingLabel-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingLIC-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingLOD-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingOpenGL-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingQt-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingVolume-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkRenderingVolumeOpenGL-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtksqlite-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtksys-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtktiff-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkverdict-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkViewsContext2D-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkViewsCore-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkViewsInfovis-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkViewsQt-7.0.lib)
win32:LIBS += $$quote(C:\Program Files\VTK\lib\vtkzlib-7.0.lib)

INCLUDEPATH += "C:\Program Files\VTK\include\vtk-7.0"
DEPENDPATH += "C:\Program Files\VTK\include\vtk-7.0"


win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKBiasCorrection-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKBioCell-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKCommon-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKDICOMParser-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkdouble-conversion-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKEXPAT-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKFEM-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmcharls-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmCommon-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmDICT-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmDSED-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmIOD-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmjpeg12-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmjpeg16-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmjpeg8-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmMEXD-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmMSFF-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmopenjpeg-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkgdcmsocketxx-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKgiftiio-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkhdf5-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkhdf5_cpp-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOBioRad-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOBMP-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOCSV-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOGDCM-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOGE-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOGIPL-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOHDF5-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOImageBase-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOIPL-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOJPEG-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOLSM-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOMesh-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOMeta-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOMRC-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIONIFTI-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIONRRD-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOPNG-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOSiemens-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOSpatialObjects-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOStimulate-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOTIFF-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOTransformBase-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOTransformHDF5-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOTransformInsightLegacy-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOTransformMatlab-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOVTK-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKIOXML-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkjpeg-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKKLMRegionGrowing-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKLabelMap-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKMesh-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKMetaIO-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itknetlib-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkNetlibSlatec-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKniftiio-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKNrrdIO-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKOptimizers-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKOptimizersv4-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKPath-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkpng-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKPolynomials-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKQuadEdgeMesh-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKSpatialObjects-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKStatistics-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itksys-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itktestlib-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itktiff-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKTransform-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkv3p_netlib-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkvcl-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKVideoCore-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKVideoIO-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkvnl-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKVNLInstantiation-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkvnl_algo-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKVTK-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKVtkGlue-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKWatersheds-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\itkzlib-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\ITKznz-4.10.lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\AdvAPI32.Lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\WS2_32.Lib)
win32:LIBS += $$quote(C:\Program Files\ITK\lib\RpcRT4.Lib)

INCLUDEPATH += "C:\Program Files\ITK\include\ITK-4.10"
DEPENDPATH += "C:\Program Files\ITK\include\ITK-4.10"
