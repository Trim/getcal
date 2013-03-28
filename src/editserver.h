#ifndef EDITSERVER_H
#define EDITSERVER_H

#include <QWidget>
#include "ui_editserver.h"
#include "icalserver.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

class EditServer : public QWidget, public Ui_EditServer
{
    Q_OBJECT

private:
    IcalServer *_server;
    bool _editionMode;
    void updateUI();
    void setConnections();

public:
    explicit EditServer(QWidget *parent = 0, Qt::WFlags f=0);
    ~EditServer();
        
public slots:
    void addServer();
    void editServer(IcalServer *server);

private slots:
    void addCalendar();
    void deleteCalendar();

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void endEdit(IcalServer *server);
};
#endif // EDITSERVER_H