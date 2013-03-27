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
#include <qpushbutton.h>
#include <qsoftmenubar.h>
#include <QMenu>

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
}

/*
 *  Destroys the object and frees any allocated resources
 */
Getcal::~Getcal()
{
    // no need to delete child widgets, Qt does it all for us
}
