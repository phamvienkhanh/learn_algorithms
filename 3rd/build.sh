#!/usr/bin/sh

root_dir=$(pwd)

# build raylib
cd ./raylib/

rm -rf build
mkdir build
cd build

cmake -G Ninja .. -DCMAKE_INSTALL_PREFIX=../../prebuilt/raylib
ninja -j4
ninja install

cd $root_dir

# build box2d
cd ./box2d/

rm -rf build
mkdir build
cd build

cmake -G Ninja .. -DCMAKE_INSTALL_PREFIX=../../prebuilt/box2d
ninja -j4
ninja install

cd $root_dir
