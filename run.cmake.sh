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

export LD_LIBRARY_PATH=$CURRENT_DIR/momo/lib:$LD_LIBRARY_PATH
# export MONO_PATH=$CURRENT_DIR/momo/lib/mono/4.5:$CURRENT_DIR/build:$MONO_PATH

sudo chown -R `whoami` $BUILD_DIR

cd build

cmake ..

make clean
make