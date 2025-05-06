#!/bin/sh

CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
PREFIX=$@
if [ -z $PREFIX ]; then
  PREFIX="$CURRENT_DIR/build"
fi

# Ensure you have write permissions to PREFIX

if [ -d $PREFIX ]; then
  echo "INFO: The Prefix directory ($PREFIX) exists already."
else
  sudo mkdir $PREFIX
fi

sudo chown -R `whoami` $PREFIX

cd build

cmake ..

make clean
make