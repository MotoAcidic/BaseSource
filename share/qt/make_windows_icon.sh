#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/ysw.png
ICON_DST=../../src/qt/res/icons/ysw.ico
convert ${ICON_SRC} -resize 16x16 ysw-16.png
convert ${ICON_SRC} -resize 32x32 ysw-32.png
convert ${ICON_SRC} -resize 48x48 ysw-48.png
convert ysw-16.png ysw-32.png ysw-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/ysw_testnet.png
ICON_DST=../../src/qt/res/icons/ysw_testnet.ico
convert ${ICON_SRC} -resize 16x16 ysw-16.png
convert ${ICON_SRC} -resize 32x32 ysw-32.png
convert ${ICON_SRC} -resize 48x48 ysw-48.png
convert ysw-16.png ysw-32.png ysw-48.png ${ICON_DST}
rm ysw-16.png ysw-32.png ysw-48.png
