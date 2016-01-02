# Add Ubuntu toolchain test PPA
# https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test 
sudo add-apt-repository ppa:ubuntu-toolchain-r/test

# Add LLVM repositories and GPG key 
# http://llvm.org/apt/ 
sudo add-apt-repository "deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty main"
sudo add-apt-repository "deb-src http://llvm.org/apt/trusty/ llvm-toolchain-trusty main"
sudo add-apt-repository "deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.6 main"
sudo add-apt-repository "deb-src http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.6 main"
sudo add-apt-repository "deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.7 main"
sudo add-apt-repository "deb-src http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.7 main" 
wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
sudo apt-get update

# Install dependencies
sudo apt-get install git git-flow mercurial subversion cmake make
sudo apt-get install g++-5 libc++abi-dev libboost1.55-all-dev
sudo apt-get install clang-3.6 lldb-3.6 
sudo apt-get install clang-3.7 lldb-3.7
sudo apt-get install clang-3.8 lldb-3.8
sudo sh ./install_libcxx.sh

# Configure compiler to Clang 3.8
sudo update-alternatives --remove-all c++
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 10




 

