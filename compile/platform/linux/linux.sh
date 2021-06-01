#!/bin/bash
# Copyright (c) 2021 MotoAcidic

###############
# Colors Keys #
###############
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

###################
# Install Depends #
###################
cd ../../
cd Depends
#cd $DEPENDS_PATH
bash LinuxDepends.sh
clear
echo VPS Server prerequisites installed.

####################
# Compile the Coin #
####################
cd ../../..
./autogen.sh
./configure --with-unsupported-ssl --with-libressl --with-gui=qt5
make

echo "Open up Winscp and connect to you vps that you compile this with. The location of the exe file is located 
	  CoinCompiled/src/qt/    The exe file will be in the qt folder and has already been striped.
	  If you need to install winscp you can get it here: https://winscp.net/eng/index.php"