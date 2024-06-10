#!/bin/bash

stash_dir=$(pwd)

# build raylib
cd ./3rd/raylib/
mkdir build
cd build

cmake .. \
	-DCMAKE_INSTALL_PREFIX=../../prebuilt/raylib

make -j4
make install

# build 
cd $stash_dir
mkdir build
cmake ..
make -j4
