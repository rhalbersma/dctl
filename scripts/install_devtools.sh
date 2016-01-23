#!/bin/bash

#
apt-get update
apt-get upgrade 
apt-get install build-essential git mercurial subversion cmake doxygen valgrind 

# Google Chrome
add-apt-repository "deb     http://dl.google.com/linux/chrome/deb/     stable main"
wget -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add -
apt-get update
apt-get install google-chrome-stable

# https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test 
add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install g++-5

# http://llvm.org/apt/ 
add-apt-repository "deb     http://llvm.org/apt/trusty/ llvm-toolchain-trusty main"
add-apt-repository "deb-src http://llvm.org/apt/trusty/ llvm-toolchain-trusty main"
wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key             | apt-key add -
apt-get update
apt-get install clang-3.8 lldb-3.8 

# Configure compilers
update-alternatives --install /usr/bin/gcc     gcc     /usr/bin/gcc-5       20
update-alternatives --install /usr/bin/gcc     gcc     /usr/bin/gcc-4.8     10
update-alternatives --install /usr/bin/g++     g++     /usr/bin/g++-5       20
update-alternatives --install /usr/bin/g++     g++     /usr/bin/g++-4.8     10
update-alternatives --install /usr/bin/clang   clang   /usr/bin/clang-3.8   10
update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 10

update-alternatives --install /usr/bin/cc      cc      /usr/bin/clang-3.8   30
update-alternatives --install /usr/bin/cc      cc      /usr/bin/gcc-5       20
update-alternatives --install /usr/bin/cc      cc      /usr/bin/gcc-4.8     10

update-alternatives --install /usr/bin/c++     c++     /usr/bin/clang++-3.8 30
update-alternatives --install /usr/bin/c++     c++     /usr/bin/g++-5       20
update-alternatives --install /usr/bin/c++     c++     /usr/bin/g++-4.8     10

# Path to manually built libraries such as libc++ and Boost
sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/local.conf'
ldconfig

#sh ./install_libcxx.sh
#sh ./install_libboost.sh





