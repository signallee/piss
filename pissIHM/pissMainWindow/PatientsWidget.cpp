#include "PatientsWidget.h"


/**
 * @brief PatientsWidget::PatientsWidget
 * @param screen_count
 * @param dispatcher
 * @param algorithmTestPlatform
 * @param patientInformationWidget
 * @param surgeryTimer
 * @param caracterStyle
 * @param appWidth
 * @param appHeight
 * @param workspaceColor
 */
PatientsWidget::PatientsWidget(int screen_count,
                               SystemDispatcher* dispatcher,
                               AlgorithmTestPlatform *algorithmTestPlatform,
                               PatientInformationWidget* patientInformationWidget,
                               QTime* surgeryTimer,
                               QFont *caracterStyle,
                               int appWidth,
                               int appHeight,
                               QString workspaceColor,
                               QString globalFontColor) : QWidget(){

    this->screen_count = screen_count;
    this->dispatcher = dispatcher;
    this->algorithmTestPlatform = algorithmTestPlatform;
    this->patientInformationWidget = patientInformationWidget;
    this->surgeryTimer = surgeryTimer;
    this->caracterStyle = caracterStyle;
    this->appWidth = appWidth;
    this->appHeight = appHeight;
    this->workspaceColor = workspaceColor;
    this->globalFontColor = globalFontColor;

    this->initVariable();
    this->constructIHM();
    this->setConnections();
    this->setWorkSpaceColor(this->workspaceColor);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::setConnections
//!
void PatientsWidget::setConnections(){
    this->connect(this->cdRomParseButton,        SIGNAL(clicked()),          this,  SLOT(doParseCdRom()));
    this->connect(this->surgeryLaunchButton,     SIGNAL(clicked()),          this,  SLOT(launchSurgery()));
    this->connect(this->addPatientButton,        SIGNAL(clicked()),          this,  SLOT(addPatient()));
    this->connect(this->leftSelectButton,        SIGNAL(clicked()),          this,  SLOT(doLeftSelect()));
    this->connect(this->rightSelectButton,       SIGNAL(clicked()),          this,  SLOT(doRightSelect()));
    this->connect(this->guidewareMovementButton, SIGNAL(clicked()),          this,  SLOT(onGuidewareMovementButtonClicked()));
    this->connect(this->cameraFlyThroughButton,  SIGNAL(clicked()),          this,  SLOT(onCameraFlyThroughButtonClicked()));
    this->connect(this->flyThroughTimer,         SIGNAL(timeout()),          this,  SLOT(flyThrough()));
    this->connect(this->cutButton,               SIGNAL(clicked()),          this,  SLOT(onCutButtonClicked()));
    this->connect(this->clearCenterLineButton,   SIGNAL(clicked()),          this,  SLOT(onClearCenterLineButtonClicked()));
    this->connect(this->cuttingLayerOption,      SIGNAL(valueChanged(int)),  this,  SLOT(cuttingLayerOptionChanged(int)));
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::launchSurgery
//!
void PatientsWidget::launchSurgery(){

    //! fetch the pointer of the patient selected, and then pass it into each component of the system
    Patient *patientHandling = this->dispatcher->getPatientById(waitingPatientsIDQueue[4].toInt(0,10));

    //! pass the pointer of the patient to the surgery planning window
    this->surgeryPlanWindow->setPatientHandling(patientHandling);
    this->surgeryPlanWindow->update();

    //! pass the pointer of the patient to the guideware tracking window
    this->guidewareTrackingWindow->setPatientHandling(patientHandling);
    this->guidewareTrackingWindow->update();

    //! to tell the piss main window to close itself
    emit surgeryLaunchButtonCicked();

    //! loading task terminated. three surgery window appear
    this->surgeryPlanWindow->displayWindow();
    this->guidewareTrackingWindow->displayWindow();
    this->controlConsoleWindow->showFullScreen();

    //! launch surgery timer
    this->surgeryTimer->start();
    this->surgeryPlanWindow->setStartTime(this->surgeryTimer->elapsed());
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::initVariable
//!
void PatientsWidget::initVariable(){
    this->numberOfPatient   = 0;
    this->dicomCDRomReader  = new DicomCDRomReader();
    this->dicomCDRomReader->setAlgorithmTestPlatform(this->algorithmTestPlatform);
    this->photoLabelStyleSheet = "border: 0px solid aliceBlue;border-radius: 0px;padding: 2 2px;background-color: transparent; color:" + globalFontColor;
    this->labelStyleSheet = "border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: "+globalFontColor;
    this->textEditStyleSheet = "border: 0px solid aliceBlue; color: "+ globalFontColor +"; background-color:"+this->workspaceColor;

    this->superviseWindow = new SuperviseWindow(appWidth, appHeight);

    this->volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
    this->volume = vtkVolume::New();
    this->renderWindow = vtkSmartPointer<vtkRenderWindow>::New() ;
    this->originVolumeDataRenderer = vtkSmartPointer<vtkRenderer>::New();
    this->volumeProperty = vtkVolumeProperty::New();
    this->volumeProperty->ShadeOff();
    this->volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(0.0,    0.0);
    compositeOpacity->AddPoint(1200.0, 0.0);
    compositeOpacity->AddPoint(1600.0, 1);
    compositeOpacity->AddPoint(2400.0, 1);
    this->volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0.0,     0.0,0.0,1.0);
    color->AddRGBPoint(1200.0,  1.0,0.0,0.0);
    color->AddRGBPoint(1600.0,  1.0,1.0,1.0);
    color->AddRGBPoint(2400.0,  1.0,1.0,1.0);
    this->volumeProperty->SetColor(color);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFixedSize(this->appWidth, this->appHeight);

    vessel = vtkPoints::New();

    flyThroughTimer = new QTimer();
    flyThroughCpt = 0;

    flyThroughRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    flyThroughRenderer = vtkSmartPointer<vtkRenderer>::New();
    flyThroughRenderWindow->AddRenderer(flyThroughRenderer);

    shiftScaleVolumeData = vtkImageShiftScale::New();

    cuttingLayerOptionActor = vtkActor::New();
    originVolumeDataCamera = this->originVolumeDataRenderer->GetActiveCamera();
    flyThroughCamera = this->flyThroughRenderer->GetActiveCamera();


    //this->flyThroughRenderer->SetActiveCamera(originVolumeDataCamera);
    //this->originVolumeDataRenderer->SetActiveCamera(flyThroughCamera);
    vesselPointCount = 0;

    centerlineActorSet.clear();
    isGuidewareMovementButtonClicked = false;

    for (int i = 0; i < 3; i++){
      riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
    }

    picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.005);

    ipwProp = vtkSmartPointer<vtkProperty>::New();

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onCutButtonClicked
//!
void PatientsWidget::onCutButtonClicked(){
    this->algorithmTestPlatform->setSystemStatus( "cut the centerline on layer: " + this->cuttingLayerOption->text()/*.toInt()*/);
    int pos = this->cuttingLayerOption->text().toInt();

    double p0[3];
    double p1[3];
    double p2[3];

    vessel->GetPoint(pos - 1, p0);
    vessel->GetPoint(pos, p1);
    vessel->GetPoint(pos + 1, p2);

    igsssCutter cutter;
    vtkActor* actor = cutter.cut1(p0, p1, p2, this->currentVolumeData);

    //this->flyThroughRenderer->AddActor(actor);
    //  flyThroughDisplayArea->update();
    //this->flyThroughRenderer->SetActiveCamera(originVolumeDataCamera);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::cuttingLayerOptionChanged
//! \param value
//!
void PatientsWidget::cuttingLayerOptionChanged(int value){
    algorithmTestPlatform->setSystemStatus(QString::number(value));
    double p0[3];
    vessel->GetPoint(value, p0);

    vtkSphereSource *pos = vtkSphereSource::New();
    pos->SetRadius(0.8);
    pos->SetThetaResolution(12);
    pos->SetPhiResolution(6);
    pos->SetCenter(p0[0],p0[1],p0[2]);

    vtkPolyDataMapper *posMapper = vtkPolyDataMapper::New();
    posMapper->SetInputConnection(pos->GetOutputPort());

    cuttingLayerOptionActor->SetMapper(posMapper);
    cuttingLayerOptionActor->GetProperty()->SetColor(255,0,0);
    this->originVolumeDataRenderer->AddActor(cuttingLayerOptionActor);
    //this->renderer->ResetCamera();
    this->flyThroughRenderer->AddActor(cuttingLayerOptionActor);
    //this->flyThroughRenderer->ResetCamera();
    currentPatientVolumeDataAnalyseArea->update();
    flyThroughDisplayArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::flyThrough
//!
void PatientsWidget::flyThrough(){

     if(!isGuidewareMovementButtonClicked){
         double p0[3];
         double foc[3];
         vessel->GetPoint(flyThroughCpt, p0);
         vessel->GetPoint(flyThroughCpt+10, foc);

         if(!flyThroughMode){
             vtkSphereSource *pos = vtkSphereSource::New();
             pos->SetRadius(0.3);
             pos->SetThetaResolution(12);
             pos->SetPhiResolution(6);
             pos->SetCenter(p0[0],p0[1],p0[2]);
             vtkPolyDataMapper *posMapper = vtkPolyDataMapper::New();
             posMapper->SetInputConnection(pos->GetOutputPort());
             vtkActor *posActor = vtkActor::New();
             posActor->SetMapper(posMapper);
             posActor->GetProperty()->SetColor(0,255,255);
             centerlineActorSet.append(posActor);
             this->originVolumeDataRenderer->AddActor(posActor);
             this->originVolumeDataRenderer->ResetCamera();
             this->flyThroughRenderer->AddActor(posActor);
             this->flyThroughRenderer->ResetCamera();
             flyThroughDisplayArea->update();
         }
         else{
             originVolumeDataCamera->SetClippingRange(0.1,5);
             originVolumeDataCamera->SetFocalPoint(foc[0],foc[1],foc[2]);
             originVolumeDataCamera->SetPosition(p0[0],p0[1],p0[2]);
             originVolumeDataCamera->ComputeViewPlaneNormal();
         }
         currentPatientVolumeDataAnalyseArea->GetRenderWindow()->Render();
         currentPatientVolumeDataAnalyseArea->update();

         flyThroughCpt += 10;
         if(flyThroughCpt >= vessel->GetNumberOfPoints() - 1){
             this->flyThroughTimer->stop();
             flyThroughCpt = 0;
             if(!flyThroughMode){
                 isGuidewareMovementButtonClicked = true;
             }
         }
     }
     else{
         if(!flyThroughMode){
             this->originVolumeDataRenderer->AddActor(centerlineActorSet.at(flyThroughCpt));
             this->originVolumeDataRenderer->ResetCamera();
             this->flyThroughRenderer->AddActor(centerlineActorSet.at(flyThroughCpt));
             this->flyThroughRenderer->ResetCamera();
             flyThroughDisplayArea->update();
             currentPatientVolumeDataAnalyseArea->update();

             flyThroughCpt++;

             if(flyThroughCpt == centerlineActorSet.size() - 1){
                 this->flyThroughTimer->stop();
                 flyThroughCpt = 0;
             }
         }
         else{
             double p0[3];
             double foc[3];
             vessel->GetPoint(flyThroughCpt, p0);
             vessel->GetPoint(flyThroughCpt+10, foc);

             originVolumeDataCamera->SetClippingRange(0.1,5);
             originVolumeDataCamera->SetFocalPoint(foc[0],foc[1],foc[2]);
             originVolumeDataCamera->SetPosition(p0[0],p0[1],p0[2]);
             originVolumeDataCamera->ComputeViewPlaneNormal();
             currentPatientVolumeDataAnalyseArea->GetRenderWindow()->Render();
             currentPatientVolumeDataAnalyseArea->update();

             flyThroughCpt += 10;
             if(flyThroughCpt >= vessel->GetNumberOfPoints() - 1){
                 this->flyThroughTimer->stop();
                 flyThroughCpt = 0;
             }
         }
     }
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onClearCenterLineButtonClicked
//!
//!    QVector <vtkActor *> centerlineActorSet;
//!    vtkActor *cuttingLayerOptionActor;
//!    vtkVolume *volume;
//!
void PatientsWidget::onClearCenterLineButtonClicked(){
    originVolumeDataRenderer->RemoveAllViewProps();
    flyThroughRenderer->RemoveAllViewProps();
    originVolumeDataRenderer->AddVolume(volume);

    this->originVolumeDataRenderer->ResetCamera();
    this->flyThroughRenderer->ResetCamera();
    currentPatientVolumeDataAnalyseArea->update();
    flyThroughDisplayArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onCameraFlyThroughButtonClicked
//!
void PatientsWidget::onCameraFlyThroughButtonClicked(){
    //! delete all the actor for center line representation if GuidewareMovementButton has been Clicked
    onClearCenterLineButtonClicked();

    flyThroughMode = true;
    if(vesselPointCount == 0){
        vesselPointCount = centerLineReader.doReadCenterLineFile("C:\\Users\\vincent\\Documents\\CanalyserWorkspace\\PatientsDataware\\Chen_Ritian__1986_02_02\\mra_tridimensionel__image\\centerlines\\reference0.txt", vessel);
        this->cuttingLayerOption->setMinimum(0);
        this->cuttingLayerOption->setMaximum(vesselPointCount-1);
    }
    this->flyThroughTimer->start(100);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onGuidewareMovementButtonClicked
//!
void PatientsWidget::onGuidewareMovementButtonClicked(){

    onClearCenterLineButtonClicked();

    flyThroughMode = false;

    if(vesselPointCount == 0){
        vesselPointCount = centerLineReader.doReadCenterLineFile("C:\\Users\\vincent\\Documents\\CanalyserWorkspace\\PatientsDataware\\Chen_Ritian__1986_02_02\\mra_tridimensionel__image\\centerlines\\reference0.txt", vessel);
        this->cuttingLayerOption->setMinimum(0);
        this->cuttingLayerOption->setMaximum(vesselPointCount-1);
    }
    this->flyThroughTimer->start(0.1);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::findPatientExisted
//!
void PatientsWidget::findPatientExisted(){
    numberOfPatient = this->dispatcher->findPatientExisted();

    //!algorithmTestPlatform->setSystemStatus(QString::number(numberOfPatient));

    for(int cpt = numberOfPatient; cpt > 0; cpt--){
        waitingPatientsPhotoStringQueue.append(this->dispatcher->getPatientById(numberOfPatient - cpt)->getPhotoPath());
        waitingPatientsStringNameQueue.append(this->dispatcher->getPatientById(numberOfPatient - cpt)->getName());
        waitingPatientsIDQueue.append(QString::number(numberOfPatient - cpt));
        waitingPatientsMraPathQueue.append(this->dispatcher->getPatientById(numberOfPatient - cpt)->getMraTridimensionelPath());
    }

    currentPatinetInfoQueue.append(this->dispatcher->getPatientById(numberOfPatient - 1)->getBirthdayOfPatient());
    //currentPatinetInfoQueue.append(this->dispatcher->getPatientById(numberOfPatient - 1)->getSexOfPatient());
    //currentPatinetInfoQueue.append(this->dispatcher->getPatientById(numberOfPatient - 1)->getIdNumber());

    this->displayLastFiveOrLess();
    this->displayCurrentPatinetInfo();
    visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::displayLastFiveOrLess
//!
void PatientsWidget::displayLastFiveOrLess(){

    int offset = numberOfPatient - 5;

    for(int cpt = 5; cpt > 0; cpt--){
        waitingPatientsQueue.at(numberOfPatient - cpt - offset)->setPixmap(waitingPatientsPhotoStringQueue.at(numberOfPatient - cpt - offset));
        waitingPatientsQueue.at(numberOfPatient - cpt - offset)->setAutoFillBackground(true);
        waitingPatientsQueue.at(numberOfPatient - cpt - offset)->setScaledContents(true);
        waitingPatientsQueue.at(numberOfPatient - cpt - offset)->update();

        waitingPatientsNameQueue.at(numberOfPatient - cpt - offset)->setText(waitingPatientsStringNameQueue.at(numberOfPatient - cpt - offset));
        waitingPatientsNameQueue.at(numberOfPatient - cpt - offset)->setAutoFillBackground(true);
        waitingPatientsNameQueue.at(numberOfPatient - cpt - offset)->setScaledContents(true);
        waitingPatientsNameQueue.at(numberOfPatient - cpt - offset)->update();
    }
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::doLeftSelect
//!
void PatientsWidget::doLeftSelect(){

    QString temp;
    temp = this->waitingPatientsPhotoStringQueue.at(0);
    this->waitingPatientsPhotoStringQueue.pop_front();
    this->waitingPatientsPhotoStringQueue.append(temp);

    QString temp1;
    temp1 = this->waitingPatientsStringNameQueue.at(0);
    this->waitingPatientsStringNameQueue.pop_front();
    this->waitingPatientsStringNameQueue.append(temp1);

    QString temp2;
    temp2 = this->waitingPatientsIDQueue.at(0);
    this->waitingPatientsIDQueue.pop_front();
    this->waitingPatientsIDQueue.append(temp2);

    QString temp3;
    temp3 = this->waitingPatientsMraPathQueue.at(0);
    this->waitingPatientsMraPathQueue.pop_front();
    this->waitingPatientsMraPathQueue.append(temp3);

    this->displayLastFiveOrLess();
    this->displayCurrentPatinetInfo();
    this->visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::doRightSelect
//!
void PatientsWidget::doRightSelect(){

    QString temp;
    temp = this->waitingPatientsPhotoStringQueue.at(this->numberOfPatient-1);

    this->waitingPatientsPhotoStringQueue.pop_back();
    this->waitingPatientsPhotoStringQueue.insert(0,temp);

    QString temp1;
    temp1 = this->waitingPatientsStringNameQueue.at(this->numberOfPatient-1);
    this->waitingPatientsStringNameQueue.pop_back();
    this->waitingPatientsStringNameQueue.insert(0,temp1);

    QString temp2;
    temp2 = this->waitingPatientsIDQueue.at(this->numberOfPatient-1);
    this->waitingPatientsIDQueue.pop_back();
    this->waitingPatientsIDQueue.insert(0,temp2);

    QString temp3;
    temp3 = this->waitingPatientsMraPathQueue.at(this->numberOfPatient-1);
    this->waitingPatientsMraPathQueue.pop_back();
    this->waitingPatientsMraPathQueue.insert(0,temp3);

    this->displayLastFiveOrLess();
    this->displayCurrentPatinetInfo();
    this->visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::displayCurrentPatinetInfo
//!
void PatientsWidget::displayCurrentPatinetInfo(){
    //this->nameLineEdit->setText(waitingPatientsStringNameQueue.at(4));
    //this->birthdayLineEdit->setText(currentPatinetInfoQueue.at(0));
    //this->sexualLineEdit->setText(currentPatinetInfoQueue.at(1));
    //this->idNumberEdit->setText(currentPatinetInfoQueue.at(1));
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::terminateSurgery
//!
void PatientsWidget::terminateSurgery(){
    this->surgeryPlanWindow->close();
    this->guidewareTrackingWindow->close();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::doParseCdRom
//!
void PatientsWidget::doParseCdRom(){
    this->algorithmTestPlatform->setSystemStatus("doParseCdRom");
    QFileInfoList drives = QDir::drives();

    for(unsigned char cpt = 0; cpt < drives.size(); cpt++){
        this->algorithmTestPlatform->setSystemStatus(drives.at(cpt).absolutePath());
    }

    dicomCDRomReader->doParseCDRom("G:/");
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::addPatient
//!
void PatientsWidget::addPatient(){
    this->patientInformationWidget->display(QCursor::pos());
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::testX
//! \return
//!
int PatientsWidget::testX(){
    return firstPatientPhotoWidget->width();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::testY
//! \return
//!
int PatientsWidget::testY(){
    return firstPatientPhotoWidget->height();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::displayBrainSegImage
//!
void PatientsWidget::visualizeCurrentVolumeData(){

    loadMRAImageFile(waitingPatientsMraPathQueue.at(4));

    this->algorithmTestPlatform->setSystemStatus(waitingPatientsMraPathQueue.at(4));
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadMRAImageFile
//! \param fileName
//! \return
//!
bool PatientsWidget::loadMRAImageFile(const QString &fileName){
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
    display(fileInterface->getImage());

    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::setWorkSpaceColor
//! \param workspaceColor
//!
void PatientsWidget::setWorkSpaceColor(QString workspaceColor){
    QColor *qworkspaceColor = new QColor(workspaceColor);

    workspaceRed = qworkspaceColor->red();
    workspaceGreen = qworkspaceColor->green();
    workspaceBlue = qworkspaceColor->blue();
    //qDebug()<<workspaceRed<<workspaceGreen<<workspaceBlue;
    this->originVolumeDataRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);
    this->flyThroughRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);

    this->setStyleSheet("background-color:"+this->workspaceColor);
    this->patientsWidgetToolBar->setStyleSheet("background-color:"+this->workspaceColor);
    this->patientsPhotoWidget->setStyleSheet("background-color:"+this->workspaceColor);
    //this->volumeDataAnalyseArea->setStyleSheet(this->textEditStyleSheet);
    this->patientInfoContainer->setStyleSheet("background-color:"+this->workspaceColor);
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::display
//! \param imgToBeDisplayed
//!
void PatientsWidget::display(vtkImageData *imgToBeDisplayed){

    this->currentVolumeData = imgToBeDisplayed;

    //! remove
    originVolumeDataRenderer->RemoveAllViewProps();

    //! generate shift scale volume data
    double range[2];
    this->currentVolumeData->GetScalarRange(range);
    this->shiftScaleVolumeData->SetShift(-1.0*range[0]);
    this->shiftScaleVolumeData->SetScale(255.0/(range[1] - range[0]));
    this->shiftScaleVolumeData->SetOutputScalarTypeToUnsignedChar();
    this->shiftScaleVolumeData->SetInputData(currentVolumeData);
    this->shiftScaleVolumeData->ReleaseDataFlagOff();
    this->shiftScaleVolumeData->Update();

    //! volume data visualization
    this->volumeMapper->SetInputData(imgToBeDisplayed);
    this->volumeMapper->SetBlendModeToMaximumIntensity();
    this->volume->SetMapper(volumeMapper);
    this->volume->SetProperty(volumeProperty);
    this->originVolumeDataRenderer->AddVolume(volume);
    this->renderWindow->AddRenderer(originVolumeDataRenderer);

    this->currentPatientVolumeDataAnalyseArea->SetRenderWindow(renderWindow);
    this->originVolumeDataRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);
    this->originVolumeDataRenderer->ResetCamera();
    this->renderWindow->Render();

    //! TODO to be verified...
    int imageDims[3];
    currentVolumeData->GetDimensions(imageDims);

    for (int i = 0; i < 3; i++){
      // make them all share the same reslice cursor object.
      vtkResliceCursorLineRepresentation *rep = vtkResliceCursorLineRepresentation::SafeDownCast(riw[i]->GetResliceCursorWidget()->GetRepresentation());
      riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

      rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

      riw[i]->SetInputData(imgToBeDisplayed);
      riw[i]->SetSliceOrientation(i);
      riw[i]->SetResliceModeToAxisAligned();
    }

    vtkRenderWindowInteractor *currentPatientVolumeDataAnalyseAreaInteractor = this->currentPatientVolumeDataAnalyseArea->GetInteractor();

//    vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];
    for (int i = 0; i < 3; i++){
      planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
      planeWidget[i]->SetInteractor(currentPatientVolumeDataAnalyseAreaInteractor);
      planeWidget[i]->SetPicker(picker);
      planeWidget[i]->RestrictPlaneToVolumeOn();
      double color[3] = {0, 0, 0};
      color[i] = 1;
      planeWidget[i]->GetPlaneProperty()->SetColor(color);

      riw[i]->GetRenderer()->SetBackground( (1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255 );

      planeWidget[i]->SetTexturePlaneProperty(ipwProp);
      planeWidget[i]->TextureInterpolateOff();
      planeWidget[i]->SetResliceInterpolateToLinear();
      planeWidget[i]->SetInputData(currentVolumeData);
      planeWidget[i]->SetPlaneOrientation(i);
      planeWidget[i]->SetSliceIndex(imageDims[i]/2);
      planeWidget[i]->DisplayTextOn();
      planeWidget[i]->SetDefaultRenderer(originVolumeDataRenderer);
      planeWidget[i]->SetWindowLevel(1358, -27);
      planeWidget[i]->On();
      planeWidget[i]->InteractionOn();
    }

    vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();

    for (int i = 0; i < 3; i++){
      cbk->IPW[i] = planeWidget[i];
      cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::WindowLevelEvent, cbk );
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk );
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResetCursorEvent, cbk );
      riw[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, cbk );

      // Make them all share the same color map.
      riw[i]->SetLookupTable(riw[0]->GetLookupTable());
      planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
      //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
      planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());

    }


    this->ResetViews();

}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::ResetViews
//! \param none
//!
void PatientsWidget::ResetViews()
{
  // Reset the reslice image views
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Reset();
    riw[i]->GetRenderer()->ResetCamera();
    }

  // Also sync the Image plane widget on the 3D top right view with any
  // changes to the reslice cursor.
  for (int i = 0; i < 3; i++)
    {
    vtkPlaneSource *ps = static_cast< vtkPlaneSource * >(
        planeWidget[i]->GetPolyDataAlgorithm());
    ps->SetNormal(riw[0]->GetResliceCursor()->GetPlane(i)->GetNormal());
    ps->SetCenter(riw[0]->GetResliceCursor()->GetPlane(i)->GetOrigin());

    // If the reslice plane has modified, update it on the 3D widget
    this->planeWidget[i]->UpdatePlacement();
    }

  // Render in response to changes.
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Render();
    }
  xzSlice->GetRenderWindow()->Render();
  riw[2]->GetRenderer()->ResetCamera();

}


//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::update
//!
void PatientsWidget::update(){

    this->patientHandling->doImageFileLecture();

    //! wait lecture finished
    do{
        this->algorithmTestPlatform->setSystemStatus("unfinished");
    }while(!this->patientHandling->readFinished());

    //this->algorithmTestPlatform->setSystemStatus("finished");
    this->visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::constructIHM
//!
void PatientsWidget::constructIHM(){

    this->patientsWidgetToolBar = new QWidget();

    this->patientsWidgetToolBar->setStyleSheet("background-color:"+this->workspaceColor);
    this->patientsWidgetToolBar->setFixedHeight(this->appHeight*0.03);

    this->simpleModeButton =  new QPushButton();
    this->simpleModeButton->setIcon(QIcon(":/images/fileList.png"));
    this->simpleModeButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->simpleModeButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->simpleModeButton->setFlat(true);

    this->patientsWidgetConfigurationButton =  new QPushButton();
    this->patientsWidgetConfigurationButton->setIcon(QIcon(":/images/patientWidgetConfiguration.png"));
    this->patientsWidgetConfigurationButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->patientsWidgetConfigurationButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->patientsWidgetConfigurationButton->setFlat(true);

    this->addPatientButton =  new QPushButton();
    this->addPatientButton->setIcon(QIcon(":/images/import_patient.png"));
    this->addPatientButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->addPatientButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->addPatientButton->setFlat(true);

    this->cdRomParseButton =  new QPushButton();
    this->cdRomParseButton->setIcon(QIcon(":/images/Disk.png"));
    this->cdRomParseButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->cdRomParseButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->cdRomParseButton->setFlat(true);

    this->controlBarSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->patientsWidgetToolBarLayout = new QHBoxLayout(this->patientsWidgetToolBar);
    this->patientsWidgetToolBarLayout->addWidget(this->simpleModeButton);
    this->patientsWidgetToolBarLayout->addWidget(this->addPatientButton);
    this->patientsWidgetToolBarLayout->addWidget(this->patientsWidgetConfigurationButton);
    this->patientsWidgetToolBarLayout->addWidget(this->cdRomParseButton);
    this->patientsWidgetToolBarLayout->addSpacerItem(this->controlBarSpacer);
    this->patientsWidgetToolBarLayout->setSpacing(0);
    this->patientsWidgetToolBarLayout->setMargin(0);

    this->upBlackArea = new QWidget();
    this->upBlackArea->setFixedHeight(this->appHeight*0.02);
    this->upBlackArea->setStyleSheet("background-color:transparent; border:0px solid beige");

    this->downBlackArea = new QWidget();
    this->downBlackArea->setFixedHeight(this->appHeight*0.02);
    this->downBlackArea->setStyleSheet("background-color:transparent;border:0px solid beige");

    this->leftBlackArea = new QWidget();
    this->leftBlackArea->setFixedWidth(this->appWidth*0.03);
    this->leftBlackArea->setStyleSheet("background-color:transparent;border:0px solid beige");

    this->rightBlackArea = new QWidget();
    this->rightBlackArea->setFixedWidth(this->appWidth*0.03);
    this->rightBlackArea->setStyleSheet("background-color:transparent;border:0px solid beige");

    this->firstPatientPhotoWidget = new PatientItem(appWidth,appHeight,this->caracterStyle);
    this->firstPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->secondPatientPhotoWidget = new PatientItem(appWidth,appHeight, this->caracterStyle);
    this->secondPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->thirdPatientPhotoWidget = new PatientItem(appWidth,appHeight, this->caracterStyle);
    this->thirdPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->fourthPatientPhotoWidget = new PatientItem(appWidth,appHeight, this->caracterStyle);
    this->fourthPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->waittingPatientPhotoes = new QWidget();
    this->waittingPatientPhotoes->setStyleSheet("background-color:transparent;border:0px solid beige");
    this->waittingPatientPhotoesLayout = new QHBoxLayout(waittingPatientPhotoes);
    this->waittingPatientPhotoesLayout->addWidget(leftBlackArea);
    this->waittingPatientPhotoesLayout->addWidget(firstPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(secondPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(thirdPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(fourthPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(rightBlackArea);
    this->waittingPatientPhotoesLayout->setSpacing(10);
    this->waittingPatientPhotoesLayout->setMargin(0);

    this->waittingPatientsAndBlackAreaWindow = new QWidget();
    this->waittingPatientsAndBlackAreaWindow->setStyleSheet("background-color:transparent; border:0px solid beige");
    this->waittingPatientsAndBlackAreaWindowLayout = new QVBoxLayout(waittingPatientsAndBlackAreaWindow);
    this->waittingPatientsAndBlackAreaWindowLayout->addWidget(upBlackArea);
    this->waittingPatientsAndBlackAreaWindowLayout->addWidget(waittingPatientPhotoes);
    this->waittingPatientsAndBlackAreaWindowLayout->addWidget(downBlackArea);
    this->waittingPatientsAndBlackAreaWindowLayout->setSpacing(0);
    this->waittingPatientsAndBlackAreaWindowLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------
    //! current patient widget
    //!--------------------------------------------------------------------------------------

    this->currentPatientInfo = new QLabel();
    this->currentPatientInfo->setStyleSheet("background-color: rgba(0, 0, 0, 100); color:"+globalFontColor);
    this->currentPatientInfo->setFont(*this->caracterStyle);
    this->currentPatientInfo->setFixedSize(this->appWidth*0.08, this->appHeight*0.03);
    this->currentPatientInfo->setAlignment(Qt::AlignCenter);

    this->patientPhotoBlankArea = new QWidget();

    this->currentPatientPhoto = new QLabel();
    this->currentPatientPhoto->setStyleSheet(this->photoLabelStyleSheet);
    this->currentPatientPhoto->setFixedSize(this->appWidth*0.08, this->appHeight*0.21);
    this->currentPatientPhotoLayout = new QVBoxLayout(this->currentPatientPhoto);
    this->currentPatientPhotoLayout->addWidget(this->currentPatientInfo);
    this->currentPatientPhotoLayout->addWidget(this->patientPhotoBlankArea);
    this->currentPatientPhotoLayout->setSpacing(0);
    this->currentPatientPhotoLayout->setMargin(0);

    this->surgeryLaunchButton = new QPushButton();
    this->surgeryLaunchButton->setText("launch");
    this->surgeryLaunchButton->setFont(QFont("Segoe UI",11,QFont::AnyStyle, false));
    this->surgeryLaunchButton->setStyleSheet("background-color:transparent;border:1px solid lightgray;color:aliceblue");
    this->surgeryLaunchButton->setFixedSize(this->appWidth*0.08, this->appHeight*0.05);

    this->patientHandlingWidget = new QWidget();
    this->patientHandlingWidget->setStyleSheet("background-color:transparent; border:0px solid aliceblue; ");
    this->patientHandlingWidget->setFixedSize(this->appWidth*0.08, this->appHeight*0.26);
    this->patientHandlingWidgetLayout = new QVBoxLayout(patientHandlingWidget);
    this->patientHandlingWidgetLayout->addWidget(currentPatientPhoto);
    this->patientHandlingWidgetLayout->addWidget(surgeryLaunchButton);
    this->patientHandlingWidgetLayout->setSpacing(0);
    this->patientHandlingWidgetLayout->setMargin(0);

    this->waitingPatientsQueue.append(this->firstPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->secondPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->thirdPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->fourthPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->currentPatientPhoto);

    this->waitingPatientsNameQueue.append(this->firstPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->secondPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->thirdPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->fourthPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->currentPatientInfo);

    //!--------------------------------------------------------------------------------------
    //!the selection buttons
    //!--------------------------------------------------------------------------------------
    this->leftSelectButton = new QPushButton();
    this->leftSelectButton->setFixedSize(this->appWidth*0.01,this->appHeight*0.26);
    this->leftSelectButton->setIcon(QIcon(":/images/splitter_left.png"));
    this->leftSelectButton->setStyleSheet("background-color:transparent; border:0px solid aliceblue; ");
    this->leftSelectButton->setFlat(true);

    this->rightSelectButton = new QPushButton();
    this->rightSelectButton->setFixedSize(this->appWidth*0.01,this->appHeight*0.26);
    this->rightSelectButton->setIcon(QIcon(":/images/splitter_right.png"));
    this->rightSelectButton->setStyleSheet("background-color:transparent; border:0px solid aliceblue;");
    this->rightSelectButton->setFlat(true);

    //!--------------------------------------------------------------------------------------
    //!patient photo widget: there are 3 photoes of waitting patients and 1 current patient
    //!--------------------------------------------------------------------------------------
    this->patientsPhotoWidget = new QLabel();
    this->patientsPhotoWidget->setFixedSize(this->appWidth*0.4, this->appHeight*0.26);
    this->patientsPhotoWidgetLayout = new QHBoxLayout(this->patientsPhotoWidget);
    this->patientsPhotoWidgetLayout->addWidget(waittingPatientsAndBlackAreaWindow);
    this->patientsPhotoWidgetLayout->addWidget(leftSelectButton);
    this->patientsPhotoWidgetLayout->addWidget(patientHandlingWidget);
    this->patientsPhotoWidgetLayout->addWidget(rightSelectButton);
    this->patientsPhotoWidgetLayout->setSpacing(0);
    this->patientsPhotoWidgetLayout->setMargin(0);
    //!--------------------------------------------------------------------------------------
    //!the information of the current patient
    //!--------------------------------------------------------------------------------------
    this->nameLabel = new QLabel("name: ");
    this->nameLabel->setFixedSize(appWidth*0.04, appHeight*0.03);
    this->nameLabel->setFont(*this->caracterStyle);
    this->nameLabel->setStyleSheet(this->labelStyleSheet);

    this->birthdayLabel = new QLabel("birth: ");
    this->birthdayLabel->setFixedSize(appWidth*0.04, appHeight*0.03);
    this->birthdayLabel->setFont(*this->caracterStyle);
    this->birthdayLabel->setStyleSheet(this->labelStyleSheet);

    this->sexualLabel = new QLabel("sex: ");
    this->sexualLabel->setFixedSize(appWidth*0.04, appHeight*0.03);
    this->sexualLabel->setFont(*this->caracterStyle);
    this->sexualLabel->setStyleSheet(this->labelStyleSheet);

    this->ageLabel = new QLabel("age: ");
    this->ageLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->ageLabel->setFont(*this->caracterStyle);
    this->ageLabel->setStyleSheet(this->labelStyleSheet);

    this->idNumberLabel = new QLabel("number: ");
    this->idNumberLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->idNumberLabel->setFont(*this->caracterStyle);
    this->idNumberLabel->setStyleSheet(this->labelStyleSheet);

    this->marryLabel = new QLabel("marriage: ");
    this->marryLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->marryLabel->setFont(*this->caracterStyle);
    this->marryLabel->setStyleSheet(this->labelStyleSheet);

    this->nationLabel = new QLabel("nationality: ");
    this->nationLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->nationLabel->setFont(*this->caracterStyle);
    this->nationLabel->setStyleSheet(this->labelStyleSheet);

    this->professionalLabel = new QLabel("professionnel: ");
    this->professionalLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->professionalLabel->setFont(*this->caracterStyle);
    this->professionalLabel->setStyleSheet(this->labelStyleSheet);

    this->leadDoctorLabel = new QLabel("doctor: ");
    this->leadDoctorLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->leadDoctorLabel->setFont(*this->caracterStyle);
    this->leadDoctorLabel->setStyleSheet(this->labelStyleSheet);

    this->therapyTimeLabel = new QLabel("therapy date: ");
    this->therapyTimeLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->therapyTimeLabel->setFont(*this->caracterStyle);
    this->therapyTimeLabel->setStyleSheet(this->labelStyleSheet);

    this->drugAllergyLabel = new QLabel("drug allergy: ");
    this->drugAllergyLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->drugAllergyLabel->setFont(*this->caracterStyle);
    this->drugAllergyLabel->setStyleSheet(this->labelStyleSheet);

    this->remarksLabel = new QLabel("ps: ");
    this->remarksLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->remarksLabel->setFont(*this->caracterStyle);
    this->remarksLabel->setStyleSheet(this->labelStyleSheet);

    this->nameLineEdit = new QLineEdit();
    this->nameLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->nameLineEdit->setFont(*this->caracterStyle);
    this->nameLineEdit->setStyleSheet(this->labelStyleSheet);

    this->birthdayLineEdit = new QLineEdit();
    this->birthdayLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->birthdayLineEdit->setFont(*this->caracterStyle);
    this->birthdayLineEdit->setStyleSheet(this->labelStyleSheet);

    this->sexualLineEdit = new QLineEdit();
    this->sexualLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->sexualLineEdit->setFont(*this->caracterStyle);
    this->sexualLineEdit->setStyleSheet(this->labelStyleSheet);

    this->ageLineEdit = new QLineEdit();
    this->ageLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->ageLineEdit->setFont(*this->caracterStyle);
    this->ageLineEdit->setStyleSheet(this->labelStyleSheet);

    this->idNumberEdit = new QLineEdit();
    this->idNumberEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->idNumberEdit->setFont(*this->caracterStyle);
    this->idNumberEdit->setStyleSheet(this->labelStyleSheet);

    this->marryLineEdit = new QLineEdit();
    this->marryLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->marryLineEdit->setFont(*this->caracterStyle);
    this->marryLineEdit->setStyleSheet(this->labelStyleSheet);

    this->nationLineEdit = new QLineEdit();
    this->nationLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->nationLineEdit->setFont(*this->caracterStyle);
    this->nationLineEdit->setStyleSheet(this->labelStyleSheet);

    this->professionalLineEdit = new QLineEdit();
    this->professionalLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->professionalLineEdit->setFont(*this->caracterStyle);
    this->professionalLineEdit->setStyleSheet(this->labelStyleSheet);

    this->leadDoctorEdit = new QLineEdit();
    this->leadDoctorEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->leadDoctorEdit->setFont(*this->caracterStyle);
    this->leadDoctorEdit->setStyleSheet(this->labelStyleSheet);

    this->therapyTimeEdit = new QLineEdit();
    this->therapyTimeEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->therapyTimeEdit->setFont(*this->caracterStyle);
    this->therapyTimeEdit->setStyleSheet(this->labelStyleSheet);

    this->drugAllergyLineEdit = new QLineEdit();
    this->drugAllergyLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->drugAllergyLineEdit->setFont(*this->caracterStyle);
    this->drugAllergyLineEdit->setStyleSheet(this->labelStyleSheet);

    this->remarksLineEdit = new QLineEdit();
    this->remarksLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->remarksLineEdit->setFont(*this->caracterStyle);
    this->remarksLineEdit->setStyleSheet(this->labelStyleSheet);

    this->patientInfoContainer = new QLabel();
    this->patientInfoContainer->setStyleSheet("background-color:red");
    this->patientInfoContainer->setFixedSize(this->appWidth*0.28, appHeight*0.26);
    this->patientInfoContainerLayout = new QGridLayout(patientInfoContainer);
    this->patientInfoContainerLayout->addWidget(nameLabel, 0, 0);
    this->patientInfoContainerLayout->addWidget(nameLineEdit, 0, 1);
    this->patientInfoContainerLayout->addWidget(birthdayLabel, 0, 2);
    this->patientInfoContainerLayout->addWidget(birthdayLineEdit, 0, 3);
    this->patientInfoContainerLayout->addWidget(sexualLabel, 1, 0);
    this->patientInfoContainerLayout->addWidget(sexualLineEdit, 1, 1);
    this->patientInfoContainerLayout->addWidget(ageLabel, 1, 2);
    this->patientInfoContainerLayout->addWidget(ageLineEdit, 1, 3);
    this->patientInfoContainerLayout->addWidget(idNumberLabel, 2, 0);
    this->patientInfoContainerLayout->addWidget(idNumberEdit, 2, 1);
    this->patientInfoContainerLayout->addWidget(marryLabel, 2, 2);
    this->patientInfoContainerLayout->addWidget(marryLineEdit, 2, 3);
    this->patientInfoContainerLayout->addWidget(nationLabel, 3, 0);
    this->patientInfoContainerLayout->addWidget(nationLineEdit, 3, 1);
    this->patientInfoContainerLayout->addWidget(professionalLabel, 3, 2);
    this->patientInfoContainerLayout->addWidget(professionalLineEdit, 3, 3);
    this->patientInfoContainerLayout->addWidget(leadDoctorLabel, 4, 0);
    this->patientInfoContainerLayout->addWidget(leadDoctorEdit, 4, 1);
    this->patientInfoContainerLayout->addWidget(therapyTimeLabel, 4, 2);
    this->patientInfoContainerLayout->addWidget(therapyTimeEdit, 4, 3);
    this->patientInfoContainerLayout->addWidget(drugAllergyLabel, 5, 0);
    this->patientInfoContainerLayout->addWidget(drugAllergyLineEdit, 5, 1);
    this->patientInfoContainerLayout->addWidget(remarksLabel, 5, 2);
    this->patientInfoContainerLayout->addWidget(remarksLineEdit, 5, 3);
    this->patientInfoContainerLayout->setSpacing(0);
    this->patientInfoContainerLayout->setMargin(0);



    //!--------------------------------------------------------------------------------------
    //! Patient's mri image display area
    //!--------------------------------------------------------------------------------------
    this->imageConfigurationAreaSpacer =  new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->guidewareMovementButton =  new QPushButton();
    this->guidewareMovementButton->setIcon(QIcon(":/images/title.png"));
    this->guidewareMovementButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->guidewareMovementButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->guidewareMovementButton->setFlat(true);

    this->cameraFlyThroughButton =  new QPushButton();
    this->cameraFlyThroughButton->setIcon(QIcon(":/images/camera.png"));
    this->cameraFlyThroughButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->cameraFlyThroughButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->cameraFlyThroughButton->setFlat(true);

    this->clearCenterLineButton =  new QPushButton();
    this->clearCenterLineButton->setIcon(QIcon(":/images/clear.png"));
    this->clearCenterLineButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->clearCenterLineButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->clearCenterLineButton->setFlat(true);

    this->imageConfigurationArea = new QLabel();
    this->imageConfigurationArea->setStyleSheet("background:"+this->workspaceColor);
    this->imageConfigurationArea->setFixedSize(this->appWidth*0.4, this->appHeight*0.04);
    this->imageConfigurationAreaLayout = new QHBoxLayout(this->imageConfigurationArea);
    this->imageConfigurationAreaLayout->addWidget(this->guidewareMovementButton);
    this->imageConfigurationAreaLayout->addWidget(this->cameraFlyThroughButton);
    this->imageConfigurationAreaLayout->addWidget(this->clearCenterLineButton);
    this->imageConfigurationAreaLayout->addItem(this->imageConfigurationAreaSpacer);
    this->imageConfigurationAreaLayout->setSpacing(0);
    this->imageConfigurationAreaLayout->setMargin(0);

    this->currentPatientVolumeDataAnalyseArea = new QVTKWidget();
    this->currentPatientVolumeDataAnalyseArea->setFixedSize(this->appWidth*0.4, this->appHeight*0.68);

    this->patientImageDispalyArea = new QLabel();
    this->patientImageDispalyArea->setFixedSize(this->appWidth*0.4, this->appHeight*0.72);
    this->patientImageDispalyAreaLayout = new QVBoxLayout(patientImageDispalyArea);
    this->patientImageDispalyAreaLayout->addWidget(this->currentPatientVolumeDataAnalyseArea);
    this->patientImageDispalyAreaLayout->addWidget(this->imageConfigurationArea);
    this->patientImageDispalyAreaLayout->setSpacing(0);
    this->patientImageDispalyAreaLayout->setMargin(0);

    //! ------------------------------------------------------------------------------------------------------------------
    //!
    //! fly thourht cam
    //!
    this->flyThroughDisplayArea = new QVTKWidget();
    this->flyThroughDisplayArea->setFixedSize(this->appWidth*0.27, this->appHeight*0.68);
    this->flyThroughDisplayArea->SetRenderWindow(flyThroughRenderWindow);

    this->flyThroughtConfigurationBarSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->cutButton =  new QPushButton();
    this->cutButton->setIcon(QIcon(":/images/couteau.png"));
    this->cutButton->setIconSize(QSize(this->appWidth*0.02,this->appHeight*0.03));
    this->cutButton->setFixedSize(this->appWidth*0.02, this->appHeight*0.03);
    this->cutButton->setFlat(true);

    this->cuttingLayerOption = new QSpinBox();
    this->cuttingLayerOption->setFixedSize(this->appWidth*0.03, this->appHeight*0.03);
    this->cuttingLayerOption->setStyleSheet("QSpinBox {background:"+this->workspaceColor+";border: 1 solide lightgray; color:beige;;selection-background-color: black;}"
                                            "QSpinBox::up-arrow {image: url(:/images/up_arrow.png);width: 5px;height: 5px;}"
                                            "QSpinBox::down-arrow {image: url(:/images/down_arrow.png);width: 5px;height: 5px; }");

    this->flyThroughtConfigurationBar = new QLabel();
    this->flyThroughtConfigurationBar->setFixedSize(this->appWidth*0.27, this->appHeight*0.04);
    this->flyThroughtConfigurationBarLayout = new QHBoxLayout(this->flyThroughtConfigurationBar);
    this->flyThroughtConfigurationBarLayout->addWidget(this->cutButton);
    this->flyThroughtConfigurationBarLayout->addWidget(this->cuttingLayerOption);
    this->flyThroughtConfigurationBarLayout->addItem(this->flyThroughtConfigurationBarSpacer);
    this->flyThroughtConfigurationBarLayout->setSpacing(0);
    this->flyThroughtConfigurationBarLayout->setMargin(0);

    this->flyThroughtArea = new QLabel();
    this->flyThroughtArea->setFixedSize(this->appWidth*0.27, this->appHeight*0.72);
    this->flyThroughtAreaLayout = new QVBoxLayout(this->flyThroughtArea);
    this->flyThroughtAreaLayout->addWidget(flyThroughDisplayArea);
    this->flyThroughtAreaLayout->addWidget(flyThroughtConfigurationBar);
    this->flyThroughtAreaLayout->setSpacing(0);
    this->flyThroughtAreaLayout->setMargin(0);

    //! TODO
    //! xy, yz, xz slice image

    xySlice = new QVTKWidget();
    xySlice->setFixedSize(this->appWidth*0.32, this->appHeight*0.32);

    yzSlice = new QVTKWidget();
    yzSlice->setFixedSize(this->appWidth*0.32, this->appHeight*0.32);

    xzSlice = new QVTKWidget();
    xzSlice->setFixedSize(this->appWidth*0.32, this->appHeight*0.32);

    //! TODO naming...
    this->xySlice->SetRenderWindow(riw[0]->GetRenderWindow());
    this->riw[0]->SetupInteractor(this->xySlice->GetRenderWindow()->GetInteractor());

    this->yzSlice->SetRenderWindow(riw[1]->GetRenderWindow());
    this->riw[1]->SetupInteractor(this->yzSlice->GetRenderWindow()->GetInteractor());

    this->xzSlice->SetRenderWindow(riw[2]->GetRenderWindow());
    this->riw[2]->SetupInteractor(this->xzSlice->GetRenderWindow()->GetInteractor());

    slicingConfigurationBar = new QLabel();
    slicingConfigurationBar->setFixedSize(this->appWidth*0.32, this->appHeight*0.04);

    analyseResultDisplayArea = new QLabel();
    analyseResultDisplayArea->setFixedWidth(this->appWidth*0.32);
    analyseResultDisplayAreaLayout = new QVBoxLayout(analyseResultDisplayArea);

    analyseResultDisplayAreaLayout->addWidget(xySlice);
    analyseResultDisplayAreaLayout->addWidget(yzSlice);
    analyseResultDisplayAreaLayout->addWidget(xzSlice);
    analyseResultDisplayAreaLayout->addWidget(slicingConfigurationBar);

    analyseResultDisplayAreaLayout->setSpacing(0);
    analyseResultDisplayAreaLayout->setMargin(0);

    //! --------------------------------------------------------------------------------------
    //! patients widget workspace
    //! --------------------------------------------------------------------------------------
    this->patientsWidgetWorkspace = new QWidget();
    this->patientsWidgetWorkspaceLayout = new QGridLayout(this->patientsWidgetWorkspace);
    this->patientsWidgetWorkspaceLayout->addWidget(patientsPhotoWidget, 0, 0);
    this->patientsWidgetWorkspaceLayout->addWidget(patientInfoContainer,0, 1);
    this->patientsWidgetWorkspaceLayout->addWidget(patientImageDispalyArea, 1, 0);
    this->patientsWidgetWorkspaceLayout->addWidget(flyThroughtArea, 1, 1);
    this->patientsWidgetWorkspaceLayout->setSpacing(0);
    this->patientsWidgetWorkspaceLayout->setMargin(0);

    this->medicalImageAnalyseArea = new QWidget();
    this->medicalImageAnalyseAreaLayout = new QHBoxLayout(medicalImageAnalyseArea);
    this->medicalImageAnalyseAreaLayout->addWidget(patientsWidgetWorkspace);
    this->medicalImageAnalyseAreaLayout->addWidget(analyseResultDisplayArea);
    this->medicalImageAnalyseAreaLayout->setSpacing(0);
    this->medicalImageAnalyseAreaLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------
    //!the layout of patients widget
    //!--------------------------------------------------------------------------------------
    patientsWidgetLayout = new QVBoxLayout(this);
    patientsWidgetLayout->addWidget(this->patientsWidgetToolBar);
    patientsWidgetLayout->addWidget(this->medicalImageAnalyseArea);
    patientsWidgetLayout->setSpacing(0);
    patientsWidgetLayout->setContentsMargins(0,0,0,0);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::addPatientToListWidget
//! \param patients
//!
void PatientsWidget::addPatientToWidget(QString path){
    QListWidgetItem *patientItem = new QListWidgetItem();
    patientItem->setIcon(QIcon(path));

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::keyPressEvent
//! \param event
//!
 void PatientsWidget::keyPressEvent(QKeyEvent *event){
     switch(event->key()){
        case Qt::Key_Left:
            this->doLeftSelect();
            break;
        case Qt::Key_Right:
            this->doRightSelect();
            break;
     }
 }

 //!----------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::drawBackground
 //! \param widget
 //! \param path
 //!
 void PatientsWidget::drawBackground(QWidget *widget, QString path, int w, int h){

     QPixmap *pixmap = new QPixmap(path);
     QPalette palette = widget->palette();

     palette.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(w, h),
                                                                  Qt::IgnoreAspectRatio,
                                                                  Qt::SmoothTransformation)));
     widget->setPalette(palette);
     widget->setMask(pixmap->mask());
 }

 //!----------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::setSurgeryPlanWindow
 //! \param surgeryPlanWindow
 //!
 void PatientsWidget::setSurgeryPlanWindow(SurgeryPlanWindow* surgeryPlanWindow){
     this->surgeryPlanWindow = surgeryPlanWindow;
 }

 //!----------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::setGuidewareTrackingWindow
 //! \param guidewareTrackingWindow
 //!
 void PatientsWidget::setGuidewareTrackingWindow(GuidewareTrackingWindow* guidewareTrackingWindow){
     this->guidewareTrackingWindow = guidewareTrackingWindow;
 }

 //!----------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::setControlConsoleWindow
 //! \param controlConsoleWindow
 //!
 void PatientsWidget::setControlConsoleWindow(ControlConsoleWindow * controlConsoleWindow){
     this->controlConsoleWindow = controlConsoleWindow;
 }

 //! ---------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::setPatientHandling
 //! \param patientHandling
 //!
 void PatientsWidget::setPatientHandling(Patient *patientHandling){
     this->patientHandling = patientHandling;
 }
