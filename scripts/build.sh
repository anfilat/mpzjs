#!/bin/bash

ROOT=`pwd`

GMP_VERSION=6.2.0
GMP=gmp-$GMP_VERSION
GMP_FILE=$GMP.tar.bz2

DEPS=$ROOT/deps
GMP_DEPS=$DEPS/gmp
BUILD=$DEPS/gmp-build-$GMP_VERSION

if [ -e "$BUILD/lib/libgmp.a" ]; then
    echo "Gmp has already been built"
    exit 0
fi

cd $DEPS
rm -rf $GMP_DEPS
tar jxf $GMP_FILE
mv $GMP $GMP_DEPS
cd $GMP_DEPS

./configure --prefix=$BUILD -enable-fat --disable-shared --with-pic CFLAGS="-fPIC"
make
if [ $? != 0 ] ; then
    echo "Unable to build gmp library"
    exit 1
fi
make install
