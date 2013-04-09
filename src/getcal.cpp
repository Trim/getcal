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
    removeProcess->start(program, arguments);

    if(!removeProcess->waitForFinished(-1)){
        qDebug()<<"Getcal : Failure while waiting end of remove script !";
    }
    qDebug()<<"Getcal : Remove process finished with status : "<<removeProcess->exitStatus();
}

void Getcal::importEvents(){
    qDebug()<<"Getcal : will import events for every server...";
    disableUi();

    QList<IcalServer> serverList = winSettings->getServers();
    int totalServer = serverList.size();
    QProgressBar * progBar = new QProgressBar();
    progBar->setMinimum(0);
    progBar->setMaximum(totalServer);
    uiMainLayout->addWidget(progBar);

    QString program = "sync4ics2openmoko.sh";

    for(int i=0;i<totalServer;++i){
        IcalServer serv = serverList[i];
        progBar->setValue(i);

        QStringList arguments;
        arguments << "-s "+ serv.getServerAddress();
        QString user = serv.getUserName();
        QString pass = serv.getPassword();
        if(!user.isEmpty() && !pass.isEmpty()){
            arguments << "-u"+user;
            arguments << "-p"+pass;
        }
        arguments << serv.getCalendars();

        QProcess *importProcess = new QProcess(this);
        qDebug()<<"Getcal : Import server "<<serv.getServerName();
        importProcess->start(program, arguments);

        if(!importProcess->waitForFinished(-1)){
            qDebug()<<"Getcal : Failure while waiting end of import script for server "<<serv.getServerName();
        }
        qDebug()<<"Getcal : Process finished with status : "<<importProcess->exitStatus()<<"for server : "<<serv.getServerName();
    }
    delete progBar;
    enableUi();
}

void Getcal::disableUi(){
    uiRemoveEvents->setDisabled(true);
    uiImportEvents->setDisabled(true);
    // Let application to process draw events, because we'll freeze it after.
    //QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}
void Getcal::enableUi(){
    uiRemoveEvents->setEnabled(true);
    uiImportEvents->setEnabled(true);
}
