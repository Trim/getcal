Getcal
======

A graphic application to store ical server configuration and import calendars using ics2qtcal perl project.

What is ics2qtcal ?
===================
It's a perl project created to parse ics files and insert events into a sqlite database generated by the QtMoko operating system.

This project has some scripts to use the parser directly for several ics files located on the web, but it won't save configuration and you have to use terminal to use it.

Althoug, ics2qtcal is _not_ a calendar synchronizer : you can only get calendars from .ics files (local files or on the web), so if you add events directly on your phone they will be _never_ saved on the web or anywhere.

As ics2qtcal is an opensource project you are free to help us to improve it (especially with recurrent events) or to take it to make a calendar synchronizer.

What is Getcal ?
================
The aim of getcal is to provide a graphical interface to ics2qtcal, so that it will save your configurations and it will be easier to execute than using terminal.

So, Getcal's aim is _not_ to provide a calendar synchronizer or a Qt/C++ implementation of ics2qtcal. Right now, it's _only_ a graphical interfarce for ics2qtcal.

Ideally, we will rewrite ics2qtcal into Qt/C++ and insert it directly in Getcal, but I know I haven't time to do it.

Although a possible improvement is to remove use of shell script and download files directly from Getcal, so it will permit to use memory instead of writting lot of files (before we implement that, we need to check if GTA02 (Neo FreeRunner) will have enough memory).

How to install Getcal ?
=======================
See [Installation](./INSTALL.md) file.

How to configure Getcal ?
=========================
See "Help" in application or [here](./help/html/getcal.html). 
