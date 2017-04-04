OFGUI-open file gui

This program displays open files
Compiled with KF5/QT5

Building
--------

Required:

compiler/make
	c++ (x++11)
	cmake
libs(+devel):
	libQt5Widgets.so.5
	libKF5CoreAddons.so.5
	libKF5ConfigCore.so.5
	libKF5I18n.so.5
	libQt5Gui.so.5
	libQt5Core.so.5
	
building:
	goto project root
	mkdir build
	cd build
	cmake ..
	make
	
This program can be installed by running  "make install"  or it can directly run from the build folder.  Ofgui can run under a 
normal user or as a root. 

Translation:
There is only a English and a Dutch translation. 
When the program is installed with "make install" and with a prefix /usr/local, it's possible that the Dutch translation doesn't work.

This can be solved when to program is build as follows:

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
make 
sudo make install

