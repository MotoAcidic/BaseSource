#!/bin/bash
# Copyright (c) 2021 MotoAcidic

HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=6
BACKTITLE="SHND WindowsCompile Wizard"
TITLE="SHND WindowsCompile Setup"
MENU="Choose either 64bit or 32bit:"

OPTIONS=(1 "Windows 64"
		 2 "Windows 32"
		 3 "Go back to OS selection menu"
		 0 "Exit Script"
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
        1) # 64bit
		cd 64
		bash ./64.sh
		;;

		2) # 32bit
		cd 32
		bash ./32.sh
		;;
		
		3) # Go back to OS selection menu
		cd ../../
		cd OS/Ubuntu-Debian/
		bash ./base_selection.sh
		;;

		0) # Exit
		Exit
		;;
esac