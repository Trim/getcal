/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

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
    QSoftMenuBar::setHelpEnabled(this,false);
    QSoftMenuBar::setInputMethodEnabled (this, false);

    winSettings = new ServerSettings(parent, f);
    menu->addAction("Settings", winSettings, SLOT(openSettings()));
    menu->addAction("Quit", this, SLOT(close()));

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

    QString program = "remove_events.pl";
    QStringList arguments;
    arguments << QTMOKO_ICALDB;

    QProcess *removeProcess = new QProcess(this);
    QObject::connect(removeProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(endRemoveEvents(int,QProcess::ExitStatus)));
    removeProcess->startDetached(program, arguments);
}

void Getcal::importEvents(){
    qDebug()<<"Getcal : will import events for every server...";
    disableUi();

    serverList = winSettings->getServers();
    totalServer = serverList.size();

    progBar = new QProgressBar();
    progBar->setMinimum(0);
    progBar->setMaximum(totalServer);
    uiMainLayout->addWidget(progBar);
    if(totalServer>0){
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
    // Let application to process draw events, because we'll freeze it after.
    //QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
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

void Getcal::importServer(int exitCode, QProcess::ExitStatus exitStatus){
    switch(exitStatus){
    case QProcess::CrashExit:{
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setText("An error occured while importing events from server CURRENT_INDEX.\nPlease try again.");
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
            arguments << "-s "+ serv.getServerAddress();
            QString user = serv.getUserName();
            QString pass = serv.getPassword();
            if(!user.isEmpty() && !pass.isEmpty()){
                arguments << "-u"+user;
                arguments << "-p"+pass;
            }

            if(!serv.getCalendars().isEmpty()){
                arguments << serv.getCalendars();
            }

            QProcess *importProcess = new QProcess(this);
            qDebug()<<"Getcal : Import server "<<serv.getServerName();
            ++currentServerImport;
            QObject::connect(importProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                             this, SLOT(importServer(int, QProcess::ExitStatus)));
            importProcess->startDetached(program, arguments);
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
