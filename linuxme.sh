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
  echo "Prefix directory exists."
else
  sudo mkdir $PREFIX
fi

sudo chown -R `whoami` $PREFIX

# Ensure that all required packages are installed.
sudo apt-get install git autoconf libtool libtool-bin automake build-essential gettext cmake python python-is-python3 2to3 libkrb5-dev libatomic-ops-dev zlib1g-dev

PATH=$PREFIX/bin:$PATH

if [ -d $TARGET_MONO_DIR_NAME ]; then
  echo "$TARGET_MONO_DIR_NAME directory already exists."
  exit 1
else
  if [ -f $REMOTE_FILENAME ]; then
    echo "$REMOTE_FILENAME exists already!"
  else
    if ! $wget $WGET_OPTS "$URL_BASE/$REMOTE_FILENAME"; then
      echo "ERROR: can't get archive $VERSION" >&2
      exit 1
    fi
  fi
fi

if [ -d $TARGET_MONO_DIR_NAME ]; then
  echo "$TARGET_MONO_DIR_NAME directory already exists."
else
  if [ -f $REMOTE_FILENAME ]; then
    tar xvf $REMOTE_FILENAME
  else
    echo "ERROR: $REMOTE_FILENAME is missing!"
    exit 1
  fi
fi

cd $TARGET_MONO_DIR_NAME

./autogen.sh --prefix=$PREFIX

make clean   # good to clean the previous builds completely
make
make install