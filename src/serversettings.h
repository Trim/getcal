#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QSettings>
#include <QMap>
#include <QStringList>
#include <QString>
#include <QDebug>

#include <qsoftmenubar.h>
#include <QMenu>
#include <QAction>

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

#include "icalserver.h"
#include "ui_serversettings.h"
#include "editserver.h"

class ServerSettings : public QWidget, public Ui_ServerSettings
        {
    Q_OBJECT
private:
    /* Settings */
    QSettings * _settings;
    QMap<QString, IcalServer> * _serverMap;
    EditServer * _editServer;
    void readServerSettings();

public:
    ServerSettings(QWidget *parent=0, Qt::WFlags f=0);
    ~ServerSettings();

public slots:
    void openSettings();
    void setServer(IcalServer *server);

private slots:
    void saveSettings();
    void editServer();

signals:
    void editClickedServer(IcalServer *server);
};
#endif // SERVERSETTINGS_H
