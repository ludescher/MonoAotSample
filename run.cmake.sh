#!/bin/sh

CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
BUILD_DIR=$@
if [ -z $BUILD_DIR ]; then
  BUILD_DIR="$CURRENT_DIR/build"
fi

if [ -d $BUILD_DIR ]; then
  echo "INFO: The directory ($BUILD_DIR) exists already."
else
  sudo mkdir $BUILD_DIR
fi

sudo chown -R `whoami` $BUILD_DIR

cd build

cmake ..

make clean
make