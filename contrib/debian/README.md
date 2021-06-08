
Debian
====================
This directory contains files used to package vkcd/vkc-qt
for Debian-based Linux systems. If you compile vkcd/vkc-qt yourself, there are some useful files here.

## vkc: URI support ##


vkc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install vkc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your vkcqt binary to `/usr/bin`
and the `../../share/pixmaps/vkc128.png` to `/usr/share/pixmaps`

vkc-qt.protocol (KDE)
