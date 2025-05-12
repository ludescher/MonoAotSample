#!/bin/bash

wget=/usr/bin/wget

CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
URL_BASE="https://download.mono-project.com/sources/mono"
WGET_OPTS=''
VERSION="6.12.0.199"
TARGET_MONO_DIR_NAME="mono-$VERSION"
REMOTE_FILENAME="mono-$VERSION.tar.xz"
PREFIX=$@
if [ -z $PREFIX ]; then
  PREFIX="$CURRENT_DIR/momo"
fi

# Ensure you have write permissions to PREFIX

if [ -d $PREFIX ]; then
  echo "INFO: The Prefix directory ($PREFIX) exists already."
else
  sudo mkdir $PREFIX
fi

sudo chown -R `whoami` $PREFIX

# Ensure that all required packages are installed.
sudo apt-get update
sudo apt-get install git autoconf libtool libtool-bin automake build-essential gettext cmake python python-is-python3 2to3 python3-distutils python3-all python3-distutils-extra libkrb5-dev libatomic-ops-dev zlib1g-dev pkg-config libicu-dev libssl-dev

PATH=$PREFIX/bin:$PATH

if [ -d $TARGET_MONO_DIR_NAME ]; then
  echo "INFO: The mono $TARGET_MONO_DIR_NAME directory exists already."
else
  if [ -f $REMOTE_FILENAME ]; then
    echo "INFO: Skip wget, because $REMOTE_FILENAME already exists!"
  else
    if ! $wget $WGET_OPTS "$URL_BASE/$REMOTE_FILENAME"; then
      echo "ERROR: Can't get archive $VERSION" >&2
      exit 1
    fi
  fi
fi

if [ -d $TARGET_MONO_DIR_NAME ]; then
  echo "INFO: The unzipped $TARGET_MONO_DIR_NAME directory exists already."
else
  if [ -f $REMOTE_FILENAME ]; then
    tar xvf $REMOTE_FILENAME
  else
    echo "ERROR: $REMOTE_FILENAME is missing!"
    exit 1
  fi
fi

export LD_LIBRARY_PATH=$CURRENT_DIR/momo/lib:$LD_LIBRARY_PATH

cd $TARGET_MONO_DIR_NAME

./autogen.sh --prefix=$PREFIX

make clean   # good to clean the previous builds completely
make
make install

cd $CURRENT_DIR/momo/lib/mono/4.5

for dll in *.dll; do
  $CURRENT_DIR/momo/bin/mono --aot=full $dll
done