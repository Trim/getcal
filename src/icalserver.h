#ifndef ICALSERVER_H
#define ICALSERVER_H

#include <QString>
#include <QStringList>
#include <QDebug>

class IcalServer: public QObject {
    Q_OBJECT

private:
    QString _serverName;
    QString _serverAddress;
    QString _userName;
    QString _userPassword;
    QStringList * _calendars;
    void setCalendars(QStringList & calendars);

public:
    /* Constructor : we need at least the server address (the name is for the GUI) */
    IcalServer();
    IcalServer(const IcalServer& icalSrv);
    IcalServer(QString serverName, QString serverAddress);
    IcalServer(QString serverName, QString serverAddress, QString userName, QString userPassword, QStringList calendars);
    ~IcalServer();
    IcalServer& operator=(const IcalServer& icalSrv);

    /* We can set a login if needed */
    QString getUserName() const;
    QString getPassword() const;

    /* Manage calendars */
    void removeCalendar(QString calendar);
    void updateCalendar(QString oldCalendar, QString newCalendar);
    QStringList &getCalendars() const;

    /* Get and modify server name and address */
    QString getServerName() const;
    QString getServerAddress() const;

    /* We'll need to check if configuration is acceptable (http://, server reachable, good http response,...)*/
    bool checkConfig();

public slots:
    void setServerName(QString name);
    void setServerAddress(QString address);
    void setUserName(QString userName);
    void setUserPass(QString userPassword);
    void addCalendar(QString calendar);
};
#endif // ICALSERVER_H
