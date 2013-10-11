#include "icalserver.h"

IcalServer::IcalServer(const IcalServer& icalSrv)
    : QObject() {
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
    // When QtMoko will work on Debian/Jessie with g++-4.8 we'll be able to replace the next four lines by calling the other constructor (see C++11) :
    //: IcalServer(serverName, serverAddress){
    {
    _serverName = serverName;
    _serverAddress = serverAddress;
    _calendars = new QStringList();
    // (stop replace)

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
    // As qtmoko-ics2qtcal add the '/' at the end of server address, we remove it here
    if(address.endsWith('/')){
        address.remove(address.lastIndexOf("/"));
    }

    _serverAddress=address;
}

void IcalServer::setUserName(QString userName){
    _userName = userName;
}

void IcalServer::setUserPass(QString userPassword){
    _userPassword = userPassword;
}

void IcalServer::addCalendar(QString calendar){
    // As qtmoko-ics2qtcal add the '/' at the beginning of calendar address, we remove it here
    if(calendar.startsWith('/')){
        calendar.remove(calendar.indexOf('/'));
    }

    _calendars->append(calendar);
}
