#ifndef GETCAL_H
#define GETCAL_H
#include "ui_getcal.h"
#include "serversettings.h"
#include <qpushbutton.h>
#include <qsoftmenubar.h>
#include <QMenu>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QProgressBar>
#include <QMessageBox>

#define QTMOKO_ICALDB "/home/root/Applications/Qtopia/qtopia_db.sqlite"

class Getcal : public QWidget, public Ui_Getcal
{
    Q_OBJECT
private:
    ServerSettings *winSettings;
    void disableUi();
    void enableUi();

    // Import specific variables
    QList<IcalServer> serverList;
    int totalServer;
    int currentServerImport;
    QProgressBar * progBar;
public:
    Getcal( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~Getcal();
private slots:
    void removeEvents();
    void endRemoveEvents(int exitCode, QProcess::ExitStatus exitStatus);
    void importEvents();
    void importServer(int exitCode=0, QProcess::ExitStatus exitStatus=QProcess::NormalExit);
};
Q_DECLARE_METATYPE(IcalServer);
#endif
