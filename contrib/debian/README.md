
Debian
====================
This directory contains files used to package yswd/ysw-qt
for Debian-based Linux systems. If you compile yswd/ysw-qt yourself, there are some useful files here.

## ysw: URI support ##


ysw-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install ysw-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your yswqt binary to `/usr/bin`
and the `../../share/pixmaps/ysw128.png` to `/usr/share/pixmaps`

ysw-qt.protocol (KDE)
