#!/bin/bash
# http://clang.llvm.org/get_started.html

# http://llvm.org/docs/GettingStarted.html#requirements
apt-get install make gcc python zlib1g
apt-get install bzip2 grep gzip sed tar unzip zip

# 2
svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm 

# 3
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
cd ../..

# 4
cd llvm/tools/clang/tools
svn co http://llvm.org/svn/llvm-project/clang-tools-extra/trunk extra
cd ../../../..

# 5
cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
cd ../..

# 6
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../llvm
make -j10

# 10
make check-clang
