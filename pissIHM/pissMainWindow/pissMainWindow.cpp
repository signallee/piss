#include "pissMainWindow.h"
#include "vtkImageData.h"


/**
 * @brief pissMainWindow::pissMainWindow
 * @param dispatcher
 */
pissMainWindow::pissMainWindow(SystemDispatcher* dispatcher): QWidget(){

    this->systemDispatcher = dispatcher;

    this->initVariable();
    this->constructIHM();
    this->globalBackgroundColorSetting();
    this->setConnections();
    this->drawBackground();
    this->onSystemWidgetOptionReleased();
}

//!---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::initVariable
//!
void pissMainWindow::initVariable(){

    //! -------------------------------------------------------
    //! Faire la decoration de l'IHM
    //! -------------------------------------------------------
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setWindowOpacity(1);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);

    //! -------------------------------------------------------
    //! Des variable utile pour manipuler l'IHM
    //! -------------------------------------------------------
    this->englishCaracterStyle = new QFont("Segoe UI", 8, QFont::AnyStyle, true);
    this->globalBackgroundColor = "dimgray";
    this->globalWorkSpaceColor = "slategray";
    this->globalFontColor = "lightgray";

    //! -------------------------------------------------------
    //! Des procedure afin de recuperer les tailles des ecrans;
    //! -------------------------------------------------------
    this->desktop = QApplication::desktop();
    this->screen_count = desktop->screenCount();
    this->primary_screen = desktop->primaryScreen();

    this->screen = new Screen[screen_count];
    for(unsigned char i = 0; i < screen_count; i++){
        this->screen[i].screenIndex = primary_screen + i;
        this->screen[i].rect = desktop->screenGeometry(primary_screen + i);
    }

    this->primary_screen_width = screen[0].rect.width();
    this->primary_screen_height = screen[0].rect.height();

    this->the_tab_style =   "QTabBar::tab{background: " + globalBackgroundColor + "; color: " + globalFontColor + "; padding: 0px; border-top: 0px solid gainsboro; border-bottom: 0px solid orange; height: "+QString::number(primary_screen_height*0.03)+"px; width: "+QString::number(primary_screen_width*0.15)+"px;  } "
                            "QTabBar::tab:selected {background: " + globalWorkSpaceColor + "; color: " + globalFontColor + "; padding: 0px; border-top: 0px solid gainsboro; border-bottom: 1px solid orange;} "
                            "QTabWidget::pane { border: 0; }";

    //!----------------------------------------------------------------------------------------------------
    //! status bar area
    //!----------------------------------------------------------------------------------------------------
    this->algorithmTestPlatform = new AlgorithmTestPlatform(this->systemDispatcher,
                                                            this->primary_screen_width,
                                                            this->primary_screen_height);

    this->patientInformationWidget = new PatientInformationWidget(this->primary_screen_width,
                                                                  this->primary_screen_height);

    this->surgeryTimer = new QTime();


    this->configuratonFilePath = "C:\\Users\\vincent\\Documents\\CanalyserWorkspace\\CanalyserMetadata\\configuration.txt";

    //!----------------------------------------------------------------------------------------------------
    //! three humain-machine interface while therapy, gauche:
    //!----------------------------------------------------------------------------------------------------
    this->surgeryPlanWindow = new SurgeryPlanWindow(this->screen[1].rect,
                                                    this->surgeryTimer,
                                                    this->systemDispatcher,
                                                    this->algorithmTestPlatform,
                                                    this->globalWorkSpaceColor,
                                                    this->configuratonFilePath);

    this->guidewareTrackingWindow = new GuidewareTrackingWindow(this->screen[2].rect,
                                                                this->systemDispatcher,
                                                                this->globalWorkSpaceColor,
                                                                this->configuratonFilePath);

    this->controlConsoleWindow = new ControlConsoleWindow(this->screen[0].rect,
                                                          this->surgeryTimer,
                                                          this->systemDispatcher,
                                                          this->algorithmTestPlatform,
                                                          this->globalWorkSpaceColor,
                                                          this->configuratonFilePath);

    this->connect(this->controlConsoleWindow, SIGNAL(missionAccomplishiment()), this, SLOT(surgeryTerminated()));

    //!----------------------------------------------------------------------------------------------------
    //! ihm pour configurer les parametre des systems
    //!----------------------------------------------------------------------------------------------------
    systemOptionWindow = new SystemOptions();
}

//!---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::getAlgorithmTestPlatform
//! \return
//!
AlgorithmTestPlatform* pissMainWindow::getAlgorithmTestPlatform(){
    return this->algorithmTestPlatform;
}

//! ---------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::globalBackgroundColorSetting
//!
void pissMainWindow::globalBackgroundColorSetting(){
    //configurationBoard->setStyleSheet("background-color:"+this->globalBackgroundColor);
    controlBoard->setStyleSheet("background-color:"+this->globalBackgroundColor);
    this->algorithmTestPlatform->setBackgroundColor(this->globalBackgroundColor);
    systemInformationBoardWidget->setStyleSheet("background-color:"+this->globalBackgroundColor);
    drawBackground();
}

//!---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::constructIHM
//!
void pissMainWindow::constructIHM(){

    //!----------------------------------------------------------------------------------------------------------------------------
    //! le page pour visualiser les informations des maladies, aussi pour choisir le bonne personne qui doit commencer le th??rapy
    //!----------------------------------------------------------------------------------------------------------------------------
    this->patientsWidget = new PatientsWidget(this->screen_count,
                                              this->systemDispatcher,
                                              this->algorithmTestPlatform,
                                              this->patientInformationWidget,
                                              this->surgeryTimer,
                                              this->englishCaracterStyle,
                                              this->primary_screen_width*0.985,
                                              this->primary_screen_height*0.79,
                                              this->globalWorkSpaceColor,
                                              this->globalFontColor);


    this->patientsWidget->setSurgeryPlanWindow(this->surgeryPlanWindow);
    this->patientsWidget->setGuidewareTrackingWindow(this->guidewareTrackingWindow);
    this->patientsWidget->setControlConsoleWindow(this->controlConsoleWindow);
    //this->patientsWidget->setParent(this); // a verifier


    //!----------------------------------------------------------------------------------------------------------------------------
    //! surveillance sur les etats du system
    //!----------------------------------------------------------------------------------------------------------------------------
    surgerySystemWidget = new SurgerySystemWidget(this->primary_screen_width*0.985,
                                                  this->primary_screen_height*0.79,
                                                  this->systemDispatcher,
                                                  this->algorithmTestPlatform,
                                                  this->globalWorkSpaceColor);

    //!----------------------------------------------------------------------------------------------------------------------------
    //! historique
    //!----------------------------------------------------------------------------------------------------------------------------
    replaysWidget = new ReplaysWidget();

    widgetsContainer = new QTabWidget();
    widgetsContainer->setFont(QFont("Segoe UI", 11, QFont::AnyStyle, true));

    widgetsContainer->setStyleSheet(the_tab_style);
    widgetsContainer->setFixedHeight(primary_screen_height*0.82);
    widgetsContainer->insertTab(0,patientsWidget, "patient");
    widgetsContainer->insertTab(1,surgerySystemWidget, "system");
    widgetsContainer->insertTab(2,replaysWidget, "documentation");

    systemInformationBoardWidget = new QWidget();
    systemInformationBoardWidgetLayout = new QVBoxLayout(systemInformationBoardWidget);
    systemInformationBoardWidgetLayout->addWidget(widgetsContainer);
    systemInformationBoardWidgetLayout->setSpacing(0);
    systemInformationBoardWidgetLayout->setMargin(0);

    //!------------------------------------------------------------------------------------------
    //! controlBoard:
    //!------------------------------------------------------------------------------------------
    closeButton = new QPushButton();
    closeButton->setIcon(QIcon(":/images/close.png"));
    closeButton->setIconSize(QSize(this->primary_screen_width*0.015,this->primary_screen_width*0.015));
    closeButton->setStyleSheet("background-color:transparent");
    closeButton->setFixedSize(this->primary_screen_width*0.015, this->primary_screen_width*0.015);

    systemConfigurationButton = new QPushButton();
    systemConfigurationButton->setIcon(QIcon(":/images/system_configuration.png"));
    systemConfigurationButton->setIconSize(QSize(this->primary_screen_width*0.015,this->primary_screen_width*0.015));
    systemConfigurationButton->setStyleSheet("background-color:transparent");
    systemConfigurationButton->setFixedSize(this->primary_screen_width*0.015, this->primary_screen_width*0.015);

    controlAreaItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    controlBoard = new QWidget();
    controlBoard->setFixedWidth(this->primary_screen_width*0.015);
    controlBoardLayout = new QVBoxLayout(controlBoard);
    controlBoardLayout->addWidget(closeButton);
    controlBoardLayout->addWidget(systemConfigurationButton);
    controlBoardLayout->addItem(controlAreaItem);
    controlBoardLayout->setSpacing(0);
    controlBoardLayout->setMargin(0);

    //!----------------------------------------------------------------------------------------------------
    //! main platform
    //!----------------------------------------------------------------------------------------------------
    mainPlatform = new QWidget();
    mainPlatformLayout = new QHBoxLayout(mainPlatform);
    //mainPlatformLayout->addWidget(configurationBoard);
    mainPlatformLayout->addWidget(systemInformationBoardWidget);
    mainPlatformLayout->addWidget(controlBoard);
    mainPlatformLayout->setSpacing(0);
    mainPlatformLayout->setMargin(0);

    //!----------------------------------------------------------------------------------------------------
    //! main window
    //!----------------------------------------------------------------------------------------------------
    igssMainWindowLayout =  new QVBoxLayout(this);
    igssMainWindowLayout->addWidget(mainPlatform);
    igssMainWindowLayout->addWidget(algorithmTestPlatform);
    igssMainWindowLayout->setSpacing(1);
    igssMainWindowLayout->setMargin(0);
}

//! ---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::findPatientExisted
//!
void pissMainWindow::findPatientExisted(){
   patientsWidget->findPatientExisted();
}

//! -----------------------------------------------------------------------------------------
//!
//! \brief AnalyserMainWindow::drawBackground
//!
void pissMainWindow::drawBackground(){
    this->setStyleSheet("background:"+this->globalWorkSpaceColor);
}

//!------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::setConnectios
//!
void pissMainWindow::setConnections(){

    this->connect(this->patientsWidget, SIGNAL(surgeryLaunchButtonCicked()), this, SLOT(close()));
    this->connect(closeButton, SIGNAL(clicked()), this, SLOT(closeSystem()));
    this->connect(systemConfigurationButton, SIGNAL(clicked()), this, SLOT(configurerLeSysteme()));
    this->connect(this->systemOptionWindow, SIGNAL(confirm()), this, SLOT(updateIHM()));

}

//!------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::updateIHM
//!
void pissMainWindow::updateIHM(){

    this->globalBackgroundColor = this->systemOptionWindow->getSystemGlobalBackgroundColor();
    this->globalWorkSpaceColor = this->systemOptionWindow->getSystemGlobalWorkSpaceColor();

    globalBackgroundColorSetting();
    widgetsContainer->setStyleSheet(the_tab_style);
    if(mainOptionStates.historyWidgetOptionState){
        this->onHistoryWidgetOptionReleased();
    }
    else if(mainOptionStates.patientsWidgetOptionState){
        this->onPatientsWidgetOptionReleased();
    }
    else if(mainOptionStates.systemWidgetOptionState){
        this->onSystemWidgetOptionReleased();
    }

    patientsWidget->setWorkSpaceColor(this->globalWorkSpaceColor);

}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::configurerLeSysteme
//!
void pissMainWindow::configurerLeSysteme(){
    systemOptionWindow->show();
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::resetConfigurationFile
//!
void pissMainWindow::resetConfigurationFile(){
    QFile f(this->configuratonFilePath);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Open failed." << endl;
    }

    QTextStream txtOutput(&f);

    txtOutput << "state:off" << endl;
    txtOutput << "name:unknown" << endl;
    txtOutput << "type:unknown" << endl;
    txtOutput << "path:unknown" << endl;

    f.close();
}

//!-------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::surgeryTerminated
//!
void pissMainWindow::surgeryTerminated(){

    this->resetConfigurationFile();

    this->controlConsoleWindow->close();

    this->patientsWidget->terminateSurgery();

    this->showFullScreen();

    this->algorithmTestPlatform->setSystemStatus("the previous therapy terminated");
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::initVisualizationComponents
//!
void pissMainWindow::initVisualizationComponents(){
    volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
    volume = vtkVolume::New();
    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::display
//!
void pissMainWindow::display(){
    this->showFullScreen();
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::setSystemDispatecher
//! \param dispatcher
//!
void pissMainWindow::setSystemDispatcher(SystemDispatcher* dispatcher){
    this->systemDispatcher = dispatcher;
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::displayMraImage
//! \param input
//!
void pissMainWindow::displayMraImage(vtkImageData* input){
    //this->volumeMapper->Set
    //this->volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    this->volumeMapper->SetInputData(input);
    this->volume->SetMapper(volumeMapper);
    this->renderer->AddVolume(volume);
    this->renderer->SetBackground(0.1, 0.21, 0.31);
    this->renderWindow->AddRenderer(renderer);
    this->renderer->ResetCamera();
    this->mraImageDisplayWindow->SetRenderWindow(this->renderWindow);
}

//!----------------------------------------------------------------------------------------
//!
//! \brief Widget::readImageFileFrom
//! \param path
//!
void pissMainWindow::readImageFileFrom(QString path){
    mhdImagePath = path;
    mraImage = new IgssImage();
    mhdFileReader = new MRAFileReader();
    mhdFileReader->doParseMHDFileToIgssImage(mhdImagePath ,mraImage);

}

//!----------------------------------------------------------------------------------------
//!
//! \brief Widget::readImageFromVtkConvert
//!
void pissMainWindow::readImageFromVtkConvert()
{
    igssImage = new IgssImage();
    vtkImage = vtkImageData::New();
    vtkImage->SetDimensions(25,25,25);
    vtkImage->SetSpacing(1.0,1.0,1.0);
    vtkImage->SetOrigin(0.0,0.0,0.0);
    vtkImage->AllocateScalars(VTK_UNSIGNED_SHORT,1);
    unsigned short *ptr = (unsigned short*)vtkImage->GetScalarPointer();
    for(long i=0;i<25*25*25;i++)
    {
        *ptr++ = i;
    }
    igssVtkImageConverter = new IgssVtkImageConverter();
    igssVtkImageConverter->VtkToIgss(vtkImage,igssImage);
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::closeSystem
//!
void pissMainWindow::closeSystem(){

    if (!(QMessageBox::information(this,tr("Igss Control View"),tr("Do you really want to log out Igss Control View?"),tr("Yes"),tr("No")))){
        this->close();
        this->guidewareTrackingWindow->close();
        this->surgeryPlanWindow->close();
        systemOptionWindow->close();
        this->surgerySystemWidget->closeSystem();
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionHovered
//!
void pissMainWindow::onPatientsWidgetOptionHovered(){
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionClicked
//!
void pissMainWindow::onPatientsWidgetOptionClicked(){
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionReleased
//!
void pissMainWindow::onPatientsWidgetOptionReleased(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionLeaved
//!
void pissMainWindow::onPatientsWidgetOptionLeaved(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionHovered
//!
void pissMainWindow::onSystemWidgetOptionHovered(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionClicked
//!
void pissMainWindow::onSystemWidgetOptionClicked(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionReleased
//!
void pissMainWindow::onSystemWidgetOptionReleased(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionLeaved
//!
void pissMainWindow::onSystemWidgetOptionLeaved(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionHovered
//!
void pissMainWindow::onHistoryWidgetOptionHovered(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionClicked
//!
//!
void pissMainWindow::onHistoryWidgetOptionClicked(){
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionReleased
//!
void pissMainWindow::onHistoryWidgetOptionReleased(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionLeaved
//!
void pissMainWindow::onHistoryWidgetOptionLeaved(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::mouseMoveEvent
//! \param event
//!
void pissMainWindow::mouseMoveEvent(QMouseEvent *event){
//    if(buttonGaucheDuSourisClique == true){
//        mouseMovingPosition = event->globalPos();
//        this->move(this->pos() + mouseMovingPosition - mousePosition);
//        mousePosition = mouseMovingPosition;
//    }
    event->ignore();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::mousePressEvent
//! \param event
//!
void pissMainWindow::mousePressEvent(QMouseEvent *event){
//    if(event->button() == Qt::LeftButton){
//       if (!((event->y() < 5) || (event->x() <5))){
//           mousePosition = event->globalPos();
//           buttonGaucheDuSourisClique = true;
//       }
//    }
    event->ignore();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::mouseReleaseEvent
//! \param event
//!
void pissMainWindow::mouseReleaseEvent(QMouseEvent *event){
//    if(event->button() == Qt::LeftButton){
//        buttonGaucheDuSourisClique = false;
//    }
    event->ignore();
}
