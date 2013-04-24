Introduction
============
The configuration of servers in Getcal is the same than ics2qtcal. The main idea is to get ics files (they can end by .ics or not) from a server passing some parameters to server.

Username and Password
=====================

To download these files, you may need to enter a _user name_ and a _password_. These parameters are _not_ mandatory.


Server URL parameter and calendars
==================================

There's a big probability that calendars (with .ics extension or not) files hosted on the same server will have a big part of their URL which is common.

The idea of _Server URL_ is to place in it this big common part of URL and to save in _Calendars_ the end of the URL which isn't shared.

Example : one server with one user
==================================

For example, if you use the Davical server on example.org you can have some calendars with this urls :

* http://cal.example.org/davical.php/user1/Personnel
* http://cal.example.org/davical.php/user1/Job.ics
* http://cal.example.org/davical.php/user1/Birthdays

In this case you'll write "http://cal.example.org/davical.php/user1" in server URL and you'll add calendars :
* Personnel
* Job.ics
* Birthdays

Example : one server with two users sharing calendars
=====================================================

Let say that there's another user, user2, which have some calendars on the same Davical server and that user1 can access user2's calendars within the server.
In this case, instead of creating two servers with same user and password, you can set "server URL" parameter as this : "http://cal.example.org/davical.php/" and add calendars :
* user1/Personnel (so the end of URL)
* user1/Job.ics
* user1/Birthdays
* _user2_/Birthdays (as it's the end of URL and user1 have access to user2's calendars)
* _user2_/Personnel
