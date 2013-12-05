#include "editserver.h"

EditServer::EditServer(QWidget *parent, Qt::WFlags f) :
    QWidget(parent, f)
{
    setupUi(this);
    qRegisterMetaType<IcalServer>("IcalServer");

    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);
    QSoftMenuBar::setHelpEnabled(this,true);
    QSoftMenuBar::setInputMethodEnabled (this, true);

    menu->addAction("Cancel", this, SLOT(cancelEdit()));
}

EditServer::~EditServer(){
    delete this;
}

void EditServer::setConnections(){
    QObject::connect(uiServerNameLine, SIGNAL(textChanged(QString)), _server, SLOT(setServerName(QString)));
    QObject::connect(uiServerNameLine, SIGNAL(textChanged(QString)), this, SLOT(updateUI()));

    QObject::connect(uiServerAddressLine, SIGNAL(editingFinished()), this, SLOT(setServerAddress()));
    QObject::connect(uiServerAddressLine, SIGNAL(editingFinished()), this, SLOT(updateUI()));

    QObject::connect(uiUserNameLine, SIGNAL(textChanged(QString)), _server, SLOT(setUserName(QString)));
    QObject::connect(uiUserNameLine, SIGNAL(textChanged(QString)), this, SLOT(updateUI()));

    QObject::connect(uiUserPassLine, SIGNAL(textChanged(QString)), _server, SLOT(setUserPass(QString)));
    QObject::connect(uiUserPassLine, SIGNAL(textChanged(QString)), this, SLOT(updateUI()));

    QObject::connect(uiAddCalendar, SIGNAL(clicked()), this, SLOT(addCalendar()));
    QObject::connect(uiDeleteCalendar, SIGNAL(clicked()), this, SLOT(deleteCalendar()));
}

void EditServer::addServer(){
    _server = new IcalServer(QString("A unique name"), QString("http://example.org/"));
    uiCalendarInput->setText("Calendar name");
    setConnections();
    updateUI();
    uiServerNameLine->setReadOnly(false);
    _cancelEdit=false;
}

void EditServer::editServer(IcalServer *server){
    // We need to copy server to be sure to not have segfault when give it back
    _server = new IcalServer(*server);
    setConnections();
    updateUI();
    uiServerNameLine->setReadOnly(true); // Prevent modification for server name because it's not really well supported right now
    _cancelEdit=false;
}

void EditServer::addCalendar(){
    QString newCal = uiCalendarInput->text();
    // Calendar name should not be empty and it shouldn't be twice in the list
    if(!newCal.isEmpty() && !_server->getCalendars().contains(newCal)){
        _server->addCalendar(newCal);
        uiCalendarInput->clear();
        updateUI();
    }
}

void EditServer::deleteCalendar(){
    QListWidgetItem *item = uiCalendarList->currentItem();
    QString cal=item->text();
    _server->removeCalendar(cal);
    delete item;
}

void EditServer::updateUI(){
    uiServerNameLine->setText(_server->getServerName());
    uiServerAddressLine->setText(_server->getServerAddress());
    uiUserNameLine->setText(_server->getUserName());
    uiUserPassLine->setText(_server->getPassword());

    uiCalendarList->clear();
    uiCalendarList->addItems(_server->getCalendars());

    if(!isVisible()){
        showMaximized();
    }
}

void EditServer::closeEvent(QCloseEvent *event){
    qDebug()<<"EditServer : end of edition for server : "<<_server->getServerName();
    if(_cancelEdit){
        qDebug()<<"EditServer : modifications cancelled";
        QWidget::closeEvent(event);
    }else{
        if(_server->getServerName().isEmpty()){
            QMessageBox* msgBox = new QMessageBox(this);
            msgBox->setText("You have to enter a name for this server.");
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->exec();
            event->ignore();
        }else{
            emit endEdit(_server);
            QWidget::closeEvent(event);
        }
    }
}

void EditServer::setServerAddress(){
    _server->setServerAddress(uiServerAddressLine->text());
}

void EditServer::cancelEdit(){
    _cancelEdit=true;
    close();
}
