To install Getcal you can use .deb packages include in download section.

Please note, that Getcal was developped on GTA04 with QtMoko compiled upon Debian/Wheezy. It should work too with GTA02 (Neo FreeRunner) and with Debian/Squeeeze but it wasn't tested.

Please be careful of the architecture : if you use GTA02 (Neo FreeRunner) you certainly need the armel version and if you use GTA04 the armhf version (it depends on your QtMoko installation).

Once you've downloaded the package, you can install it with :
* dpkg -i getcal.deb
* apt-get install -f

It's suggested to do it using wifi or usb network, because Getcal have some dependancies on perl and external libraries, so it need to download some Mb of data.

Later we'll provide the application directly on QtMoko servers and it will be installable using "More Apps" application.
