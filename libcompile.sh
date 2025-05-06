#!/bin/bash

CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

cd $CURRENT_DIR/momo/lib/mono/4.5

for dll in *.dll; do
  $CURRENT_DIR/momo/bin/mono --aot=full $dll
done