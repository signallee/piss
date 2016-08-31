#include <QApplication>
#include "pissMainWindow.h"
#include "SystemDispatcher.h"
#include "SystemDataBase.h"
#include "pissCommunicationStack.h"
#include "GlobalTime.h"


/**
 * @brief main procedure to generate application
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    //! -------------------------------------------------------------------------------------------
    //! application
    //! -------------------------------------------------------------------------------------------
    QApplication analyser(argc, argv);
    analyser.setOrganizationName("CAS - SIAT");
    analyser.setApplicationName("piss");
    analyser.setWindowIcon(QIcon(":/images/icon.png"));

    //! -------------------------------------------------------------------------------------------
    //! TODO: global timer to be defined....
    //! -------------------------------------------------------------------------------------------
    GlobalTime *globalTime = new GlobalTime();

    //! -------------------------------------------------------------------------------------------
    //! le base de donn¨¦es pour enregistrer les informations qui concerne des maladies
    //! -------------------------------------------------------------------------------------------
    SystemDataBase* database = new SystemDataBase();

    //! -------------------------------------------------------------------------------------------
    //! l'ordonnanceur du syst¨¨me
    //! -------------------------------------------------------------------------------------------
    SystemDispatcher* dispatcher = new SystemDispatcher();

    //! -------------------------------------------------------------------------------------------
    //! l'IHM
    //! -------------------------------------------------------------------------------------------
    pissMainWindow*   mainWindow = new pissMainWindow(dispatcher);

    //! -------------------------------------------------------------------------------------------
    //! le biblioth¨¦que des algorithmes pour faire le traitement des images
    //! -------------------------------------------------------------------------------------------
    ImageProcessingFactory *imageProcessingFactory = new ImageProcessingFactory();

    //! -------------------------------------------------------------------------------------------
    //! Initialiser l'environment du r¨¦seaux interne pour contruire le syst¨¨me r¨¦parti
    //! -------------------------------------------------------------------------------------------
    pissCommunicationStack *communicationStack = new pissCommunicationStack(globalTime);

    //! -------------------------------------------------------------------------------------------
    //! Cr¨¦er les liens entre les diff¨¦rents modules du logiciel de navigateur
    //! -------------------------------------------------------------------------------------------
    dispatcher->setSystemDataBase(database);
    dispatcher->setImageProcessingFactory(imageProcessingFactory);
    dispatcher->setCommunicationStack(communicationStack);

    database->setImageProcessingFactory(imageProcessingFactory);
    communicationStack->setDatabase(database);

    //! -------------------------------------------------------------------------------------------
    //! Chercher les maladies existants
    //! -------------------------------------------------------------------------------------------
    mainWindow->findPatientExisted();

    //! -------------------------------------------------------------------------------------------
    //! Lancer l'IHM principale
    //! -------------------------------------------------------------------------------------------
    mainWindow->display();

    return analyser.exec();
}
