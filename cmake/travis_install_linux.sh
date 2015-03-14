#!/bin/bash

sudo apt-get -qq -y install libsdl2-dev libsdl2-ttf-dev libalut-dev libpng12-dev cmake libglm-dev

if [ "$CC" = "gcc" ]; then sudo apt-get install gcc-4.8; fi
if [ "$CXX" = "g++" ]; then sudo apt-get install g++-4.8; fi

if [ "$CXX" = "clang++" ]; then sudo apt-get install --allow-unauthenticated clang-3.5; fi

git clone https://github.com/hpicgs/glbinding.git
cd glbinding
cmake .
make -j4
sudo make install
cd ..

