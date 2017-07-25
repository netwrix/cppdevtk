#!/bin/bash


if [ $EUID -ne 0 ]; then
	echo "This script must be run as root!"
	exit 1
fi


# TODO: modify as needed
SENSITIVITYIO_LIBDIR="/opt/cososys/local/x86_64/lib64"
QT_DIR="/Applications/Qt/5.6/clang_64"


cp -f -R -P $QT_DIR/lib/QtCore.framework $SENSITIVITYIO_LIBDIR
cp -f -R -P $QT_DIR/lib/QtNetwork.framework $SENSITIVITYIO_LIBDIR
cp -f -R -P $QT_DIR/lib/QtPrintSupport.framework $SENSITIVITYIO_LIBDIR
cp -f -R -P $QT_DIR/lib/QtGui.framework $SENSITIVITYIO_LIBDIR
cp -f -R -P $QT_DIR/lib/QtWidgets.framework $SENSITIVITYIO_LIBDIR

mkdir -p $SENSITIVITYIO_LIBDIR/plugins/bearer
cp -f $QT_DIR/plugins/bearer/libqcorewlanbearer.dylib $SENSITIVITYIO_LIBDIR/plugins/bearer
cp -f $QT_DIR/plugins/bearer/libqgenericbearer.dylib $SENSITIVITYIO_LIBDIR/plugins/bearer
mkdir -p $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqdds.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqgif.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqicns.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqico.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqjpeg.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqtga.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqtiff.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqwbmp.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
cp -f $QT_DIR/plugins/imageformats/libqwebp.dylib $SENSITIVITYIO_LIBDIR/plugins/imageformats
mkdir -p $SENSITIVITYIO_LIBDIR/plugins/platforms
cp -f $QT_DIR/plugins/platforms/libqcocoa.dylib $SENSITIVITYIO_LIBDIR/plugins/platforms
mkdir -p $SENSITIVITYIO_LIBDIR/plugins/printsupport
cp -f $QT_DIR/plugins/printsupport/libcocoaprintersupport.dylib $SENSITIVITYIO_LIBDIR/plugins/printsupport


echo "Done"
