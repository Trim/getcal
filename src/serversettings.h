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
#include <QListWidgetItem>
#include <QSize>
#include <QPushButton>

#include "icalserver.h"
#include "ui_serversettings.h"
#include "editserver.h"

#define SETTINGS_SERVER_GRP "servers"
#define SETTINGS_SERVER_LIST_HEIGHT 80

/*
 * The ServerSettings class is class which save and load settings
 * and display the loaded servers in a list to modify them with
 * the EditServer class.
 */
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
    QList<IcalServer> getServers();

private:
    void insertServer(QString srvName);

public slots:
    void openSettings();
    void setServer(IcalServer *server);

private slots:
    void saveSettings();
    void editServer();
    void deleteServer();

signals:
    void editClickedServer(IcalServer *server);
};
#endif // SERVERSETTINGS_H
