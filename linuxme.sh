#!/bin/bash

PREFIX=$@
if [ -z $PREFIX ]; then
  PREFIX="/home/johannes/Playground/cppwithcsharp/momo-heaven"
fi

# Ensure you have write permissions to PREFIX

if [ -d $PREFIX ]; then
  echo "Prefix directory exists."
else
  sudo mkdir $PREFIX
fi

sudo chown -R `whoami` $PREFIX

# Ensure that all required packages are installed.
sudo apt-get install git autoconf libtool automake build-essential gettext cmake python libkrb5-dev libatomic-ops-dev zlib1g-dev

PATH=$PREFIX/bin:$PATH

if [ -d momomain ]; then
  echo "momomain directory exists."
else
  git clone https://github.com/mono/mono.git momomain
fi

cd momomain

git checkout mono-6.12.0.200

./autogen.sh --prefix=$PREFIX

make clean   # good to clean the previous builds completely
make
make install