#include "editserver.h"

EditServer::EditServer(QWidget *parent, Qt::WFlags f) :
    QWidget(parent, f)
{
    setupUi(this);
    qRegisterMetaType<IcalServer>("IcalServer");
}

EditServer::~EditServer(){
    delete this;
}

void EditServer::setConnections(){
    QObject::connect(uiServerNameLine, SIGNAL(textChanged(QString)), _server, SLOT(setServerName(QString)));
    QObject::connect(uiServerAddressLine, SIGNAL(textChanged(QString)), _server, SLOT(setServerAddress(QString)));
    QObject::connect(uiUserNameLine, SIGNAL(textChanged(QString)), _server, SLOT(setUserName(QString)));
    QObject::connect(uiUserPassLine, SIGNAL(textChanged(QString)), _server, SLOT(setUserPass(QString)));
    QObject::connect(uiAddCalendar, SIGNAL(clicked()), this, SLOT(addCalendar()));
    QObject::connect(uiDeleteCalendar, SIGNAL(clicked()), this, SLOT(deleteCalendar()));
}

void EditServer::addServer(){
    _server = new IcalServer(QString("A unique name"), QString("http://example.org/"));
    uiCalendarInput->setText("Calendar name");
    setConnections();
    updateUI();
    uiServerNameLine->setReadOnly(false);
}

void EditServer::editServer(IcalServer *server){
    // We need to copy server to be sure to not have segfault when give it back
    _server = new IcalServer(*server);
    setConnections();
    updateUI();
    uiServerNameLine->setReadOnly(true); // Prevent modification for server name because it's not really well supported right now
}

void EditServer::addCalendar(){
    QString newCal = uiCalendarInput->text();
    // Calendar name should not be empty and it shouldn't be twice in the list
    if(!newCal.isEmpty() && !_server->getCalendars().contains(newCal)){
        uiCalendarList->addItem(newCal);
        _server->addCalendar(newCal);
        uiCalendarInput->clear();
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
    qDebug()<<"EditServer : will emit signal endEdit with server : "<<_server->getServerName();
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
