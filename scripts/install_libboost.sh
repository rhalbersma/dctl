#!/bin/bash
apt-get install libicu-dev libbz2-dev zlib1g-dev python-dev libopenmpi-dev
wget http://downloads.sourceforge.net/boost/boost_1_60_0.tar.bz2
tar --bzip2 -xf boost_1_60_0.tar.bz2
cd boost_1_60_0/
./bootstrap.sh --with-toolset=clang --with-libraries=all
./b2 -j10 toolset=clang --variant=debug,release --link=shared --runtime-link=shared --threading=multi cxxflags="-std=c++1z" linkflags="-lc++abi"
./b2 -j10 install

