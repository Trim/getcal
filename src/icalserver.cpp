#include "icalserver.h"

IcalServer::IcalServer(const IcalServer& icalSrv){
    _serverName = icalSrv.getServerName();
    _serverAddress = icalSrv.getServerAddress();
    _userName = icalSrv.getUserName();
    _userPassword = icalSrv.getPassword();
    _calendars =  new QStringList(icalSrv.getCalendars());
}

IcalServer& IcalServer::operator=(const IcalServer& icalSrv) {
    if ( this == &icalSrv ) {
        return *this; //Self assignment : nothing to do
    }

    _serverName = icalSrv.getServerName();
    _serverAddress = icalSrv.getServerAddress();
    _userName = icalSrv.getUserName();
    _userPassword = icalSrv.getPassword();
    _calendars =  new QStringList(icalSrv.getCalendars());
    return *this;
}

IcalServer::IcalServer(QString serverName, QString serverAddress)
{
    _serverName = serverName;
    _serverAddress = serverAddress;
    _calendars = new QStringList();
}

IcalServer::IcalServer(QString serverName,
                       QString serverAddress,
                       QString userName,
                       QString userPassword,
                       QStringList calendars)
    : IcalServer(serverName, serverAddress){
    this->setUserName(userName);
    this->setUserPass(userPassword);
    this->setCalendars(calendars);
}


IcalServer::IcalServer(){
}

IcalServer::~IcalServer(){
}

void IcalServer::setCalendars(QStringList & calendars){
    _calendars = new QStringList(calendars);
}

QString IcalServer::getUserName() const{
    return _userName;
}

QString IcalServer::getPassword() const{
    return _userPassword;
}

void IcalServer::removeCalendar(QString calendar){
    _calendars->removeAll(calendar);
}

void IcalServer::updateCalendar(QString oldCalendar, QString newCalendar){
    _calendars->replace(_calendars->indexOf(oldCalendar), newCalendar);
}

QStringList &IcalServer::getCalendars() const{
    return *_calendars;
}

bool IcalServer::checkConfig(){
    return true;
}

QString IcalServer::getServerName() const{
    return _serverName;
}

QString IcalServer::getServerAddress() const{
    return _serverAddress;
}

/* Public slots */
void IcalServer::setServerName(QString name){
    _serverName=name;
}

void IcalServer::setServerAddress(QString address){
    _serverAddress=address;
}

void IcalServer::setUserName(QString userName){
    _userName = userName;
}

void IcalServer::setUserPass(QString userPassword){
    _userPassword = userPassword;
}

void IcalServer::addCalendar(QString calendar){
    _calendars->append(calendar);
}
