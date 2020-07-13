#!/bin/bash

LIBZ_NAME=
if [[ "$OSTYPE" == "darwin"* ]]; then
  LIBZ_NAME=libz.dylib
else
  LIBZ_NAME=libz.so
fi

SOURCES=`pwd`/../../sources

ZLIB_INCLUDE=$SOURCES/SDL_image/external/zlib-1.2.11/include
ZLIB_LIBRARY=$SOURCES/SDL_image/external/zlib-1.2.11/build/lib/$LIBZ_NAME

BUILD_DIR=./build
if [[ -d "${BUILD_DIR}" && ! -L "${BUILD_DIR}" ]]; then
  cd build
else
  mkdir build
  cd build
fi

cmake -DZLIB_INCLUDE=$ZLIB_INCLUDE -DZLIB_LIBRARY=$ZLIB_LIBRARY $SOURCES

make

cd ..

