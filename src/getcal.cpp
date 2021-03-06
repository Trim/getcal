#include "getcal.h"

/*
 *  Constructs a Example which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
Getcal::Getcal(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    setupUi(this); // reads the .ui file to determine widgets and layout

    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);
    QSoftMenuBar::setHelpEnabled(this,true);
    QSoftMenuBar::setInputMethodEnabled (this, false);

    winSettings = new ServerSettings(parent, f);
    menu->addAction("Settings", winSettings, SLOT(openSettings()));
    menu->addAction("Quit", this, SLOT(close()));
    mokoEnv = QProcessEnvironment::systemEnvironment();
    mokoEnv.insert("PATH", mokoEnv.value("PATH") + ":/opt/qtmoko/bin");

    QObject::connect(uiImportEvents, SIGNAL(clicked()), this, SLOT(importEvents()));
    QObject::connect(uiRemoveEvents, SIGNAL(clicked()), this, SLOT(removeEvents()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
Getcal::~Getcal()
{
    // no need to delete child widgets, Qt does it all for us
}

void Getcal::removeEvents(){
    qDebug()<<"Getcal : will remove events...";
    disableUi();

    QString program = "deleteqtcalappointments.pl";
    QStringList arguments;
    arguments << QTMOKO_ICALDB;

    QProcess *removeProcess = new QProcess(this);
    QObject::connect(removeProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(endRemoveEvents(int,QProcess::ExitStatus)));
    removeProcess->setProcessEnvironment(mokoEnv);
    removeProcess->start(program, arguments);
}

void Getcal::importEvents(){
    qDebug()<<"Getcal : will import events for every server...";
    disableUi();

    serverList = winSettings->getServers();
    totalServer = serverList.size();

    if(totalServer>0){
        progBar = new QProgressBar();
        progBar->setMinimum(0);
        progBar->setMaximum(totalServer);
        uiMainLayout->addWidget(progBar);

        currentServerImport=0;
        importServer();
    }else{
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setText("No server has been configured, so nothing has been done.\nPlease create at least one server.");
        msgBox->setIcon(QMessageBox::Information);
        msgBox->exec();
        enableUi();
    }
}

void Getcal::disableUi(){
    uiRemoveEvents->setDisabled(true);
    uiImportEvents->setDisabled(true);
    QMenu *menu = QSoftMenuBar::menuFor(this);
    menu->setDisabled(true);
}

void Getcal::enableUi(){
    uiRemoveEvents->setEnabled(true);
    uiImportEvents->setEnabled(true);
    QMenu *menu = QSoftMenuBar::menuFor(this);
    menu->setEnabled(true);
}

void Getcal::endRemoveEvents(int exitCode, QProcess::ExitStatus exitStatus){
    switch(exitStatus){
    case QProcess::CrashExit:{
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setText("An error occured while removing events.\nPlease try again.");
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->exec();
        enableUi();
        break;
    }
    case QProcess::NormalExit:{
        enableUi();
        break;}
    default:{
        enableUi();
        break;
    }
    }
}

/* This method is a recursive function which uses the values currentServerImport and the number of
 * configured servers (totalServer) to determine its end. Please initalize currentServerImport before
 * you use this method.
 */
void Getcal::importServer(int exitCode, QProcess::ExitStatus exitStatus){
    switch(exitStatus){
    case QProcess::CrashExit:{
        QMessageBox* msgBox = new QMessageBox(this);
        QString* message = new QString("An error occured while importing events from server ");
        message->append(serverList[currentServerImport].getServerName());
        message->append("\nPlease try again.");
        msgBox->setText(*message);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->exec();
        enableUi();
        break;
    }
    case QProcess::NormalExit:{
        if(currentServerImport<totalServer && currentServerImport>=0){
            IcalServer serv = serverList[currentServerImport];
            progBar->setValue(currentServerImport);

            QString program = "sync4ics2openmoko.sh";
            QStringList arguments;

            QString srvName = serv.getServerName();
            QString addr = serv.getServerAddress();
            QString user = serv.getUserName();
            QString pass = serv.getPassword();

            if(!user.isEmpty() && !pass.isEmpty()){
                arguments << "-u"+user;
                arguments << "-p"+pass;
            }

            if(serv.getCalendars().isEmpty()){
                arguments << addr;
            }else{
                arguments << "-s "+addr;
                arguments << serv.getCalendars();
            }

            QProcess *importProcess = new QProcess(this);
            qDebug()<<"Getcal : Import server "<<srvName;

            ++currentServerImport;
            QObject::connect(importProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                             this, SLOT(importServer(int, QProcess::ExitStatus)));

            importProcess->setProcessEnvironment(mokoEnv);
            importProcess->setStandardErrorFile("/tmp/getcal_sync_"+srvName+".err");
            importProcess->setStandardOutputFile("/tmp/getcal_sync_"+srvName+".log");

            importProcess->start(program, arguments);
        }else{
            delete progBar;
            enableUi();
        }
        break;}
    default:{
        enableUi();
        break;
    }
    }
}
