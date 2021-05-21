#!/bin/bash
# Copyright (c) 2021 MotoAcidic

# Check for supported OS to run the install
if [ -n "$(. /etc/os-release; echo $NAME | grep -i Ubuntu)" -o -n "$(. /etc/os-release; echo $NAME | grep -i Debian)" ]; then
# OS is supported so we go to the menu selection.
cd OS/Ubuntu-Debian/
bash base_selection.sh
else

HEIGHT=15
WIDTH=50
CHOICE_HEIGHT=6
BACKTITLE="OS Check"
TITLE="OS Check"
MENU="Only tested with Ubuntu - Debian currently do you wish to continue?"

OPTIONS=(1 "Yes"
		 2 "No"
)

CHOICE=$(whiptail --clear\
		--backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)
clear

case $CHOICE in
        1) # Install the ubuntu and debian defualt installer
		cd OS/Ubuntu-Debian/
		bash base_selection.sh
		;;

		2) # Exit the script
		exit
		;;
esac
fi