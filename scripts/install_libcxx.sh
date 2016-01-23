#!/bin/bash
#http://libcxx.llvm.org/docs/BuildingLibcxx.html
svn --quiet co http://llvm.org/svn/llvm-project/llvm/trunk llvm
cd llvm
svn --quiet co http://llvm.org/svn/llvm-project/libcxx/trunk projects/libcxx
svn --quiet co http://llvm.org/svn/llvm-project/libcxxabi/trunk projects/libcxxabi
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
make -j10 cxx
make -j10 install-libcxx install-libcxxabi

