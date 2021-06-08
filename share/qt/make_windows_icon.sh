#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/vkc.png
ICON_DST=../../src/qt/res/icons/vkc.ico
convert ${ICON_SRC} -resize 16x16 vkc-16.png
convert ${ICON_SRC} -resize 32x32 vkc-32.png
convert ${ICON_SRC} -resize 48x48 vkc-48.png
convert vkc-16.png vkc-32.png vkc-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/vkc_testnet.png
ICON_DST=../../src/qt/res/icons/vkc_testnet.ico
convert ${ICON_SRC} -resize 16x16 vkc-16.png
convert ${ICON_SRC} -resize 32x32 vkc-32.png
convert ${ICON_SRC} -resize 48x48 vkc-48.png
convert vkc-16.png vkc-32.png vkc-48.png ${ICON_DST}
rm vkc-16.png vkc-32.png vkc-48.png
