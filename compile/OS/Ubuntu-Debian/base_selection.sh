#!/bin/bash
# Copyright (c) 2021 MotoAcidic

HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=6
BACKTITLE="SHND Compile Wizard"
TITLE="SHND Compile Wizard"
MENU="Choose one of the following bases to compile from:"

OPTIONS=(1 "Compile Windows"
		 2 "Compile Linux"

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
        0)  # Exit
		exit	
		;;

        1)	# Windows compile
		cd ../../
		cd platform/win/
		bash win.sh
        ;; 

		2)	# Linux Compile
		cd ../../
		cd platform/linux/
		bash linux.sh
        ;;

esac