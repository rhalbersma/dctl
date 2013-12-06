DCTL: Draughts and Checkers Template Library
============================================

A [C++](http://isocpp.org) template library for the games of [draughts](http://en.wikipedia.org/wiki/Draughts) and [checkers](http://en.wikipedia.org/wiki/Checkers_(disambiguation)), supporting almost all known rules variations, board sizes and game objectives.

* **DISCLAIMER:** This library is currently in a [pre-alpha](https://en.wikipedia.org/wiki/Software_release_life_cycle#Pre-alpha) stage. All code is subject to change without prior notice. 

Introduction
------------

Draughts is a family of related board games played in many [regions](http://alemanni.pagesperso-orange.fr/geography.html) across the world. There are many country-specific game rules such as International/Brazilian/Canadian, American/English, Russian, Italian, Spanish/Portugese, Czech, Thai, and Frisian draughts. Draughts is played on checkered boards of dimensions 8x8, 10x10 and 12x12. The common game objective is "the first player to have no moves left, **loses**". A less common variation is so-called misère play ("first to have no moves left, **wins**"), also known as "suicide", "giveaway", or "qui-perd-gagné". Many draughts variants have an official governing body, such as the [Fédération Mondiale du Jeu de Dames](htpp://fmjd.org) (FMJD, World Draughts Federation).

Scope
-----

The DCTL supports all above mentioned draughts variants with official rules, on all board sizes smaller than 12x12. Most existing draughts variants have a high percentage of draws when played between experienced players. Historically, many small tweaks to the game rules have been proposed. Even former world champions such as [Herman Hoogland](http://en.wikipedia.org/wiki/Herman_Hoogland) proposed new variants. The DCTL supports some of these variants. In particular, the recently proposed [Killer Draughts](http://www.killerdraughts.org/) is actively supported.

Usage
-----

The DCTL provides a series of [C++ templates](http://en.wikipedia.org/wiki/Template_(C%2B%2B)) to define the board geometry and game rules of draughts variants. With these templates, one can read and write draughts positions from so-called FEN strings using [Portable Draughts Notation](http://www.10x10.dse.nl/pdn/introduction.html). Once a position variable has been initialized with such a FEN string, the DCTL provides further templates to analyze them using a search algorithm, which is supported by a typical board game [engine infrastructure](http://chessprogramming.wikispaces.com/) of e.g. a move generator, evaluation function and hash table.

* **TIP**: Consult the test-suite sources (under the `dctl/test` directory) for concrete usage examples.

Road-map
-------

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. It is currently not possible to play a game against a program based on the DCTL. Functionality being planned includes:   

* support for 12x12 and larger boards
* a generic engine class template   
* support for the [Checkerboard GUI](http://www.fierz.ch/cbdeveloper.php)   
* support for the [GUIDE protocol](http://laatste.info/bb3/download/file.php?id=864)   
* support for the [DamExchange protocol](http://www.mesander.nl/damexchange/edxpmain.htm)  
* endgame database generation   

Requirements
------------

### Platforms

The DCTL aims to be cross-platform in the near future, but is currently only supported on .deb based 64-bit Linux distributions (Mint, Ubuntu, Debian) with at least libstdc++6 from g++ >= 4.8.1 (i.e. Mint >= 16, Ubuntu >= 13.10, Debian >= jessie). Note that libstdc++6 comes pre-installed on almost every Linux distribution. The following commands get all the other requirements:

      # Get a fresh system and install build tools and pre-compiled Boost Libraries
      sudo apt-get update
      sudo apt-get install tortoisehg cmake make libboost1.54-all-dev

      # Patch libstdc++ <cstdio> header so that ::gets is removed for C++14
      sudo sed -i '/using ::gets;/c\#if __cplusplus <= 201103L\n\using ::gets;\n\#endif' /usr/bin/../lib/gcc/x86_64-linux-gnu/4.8/../../../../include/c++/4.8/cstdio
      
      # Add LLVM repository and GPG key and install Clang 3.5
      echo "deb http://llvm.org/apt/saucy/ llvm-toolchain-saucy main" | sudo tee -a /etc/apt/sources.list.d/llvm.list
      wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
      sudo apt-get install clang-3.5 lldb-3.5

      # Patch clang 3.5 so that the include directory no longer points to llvm-3.4
      cd /usr/lib/clang/3.5
      sudo ln -sf ../../llvm-3.5/lib/clang/3.5/include/ include

### Compilers

The DCTL is a modern [C++](http://isocpp.org) library that targets the upcoming [C++14 Standard](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3797.pdf). This currently restricts usage of the DCTL to the latest stable version of [Clang](http://clang.llvm.org/cxx_status.html). Note that the clang-3.4 package on Mint 16 and Ubuntu 13.10 does *NOT* correspond to the official Clang 3.4 release, but to a June 2013 SVN version that is not C++14 feature-complete. Instead, download the nightly build for the Clang 3.5 release series. 

### Boost headers

The DCTL uses several of the popular [Boost C++ libraries](http://www.boost.org). Current development takes place with Boost 1.54.0. Boost is a collection of header-only libraries, and you simply have to point your compiler to the Boost include directory. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_54_0/more/getting_started/index.html) on how to do this on your system. After that, you can continue to use your regular build process.

### Boost libraries

The test-suite uses [Boost.Test](http://www.boost.org/doc/libs/1_54_0/libs/test/doc/html/index.html). In order to build and run the test-suite (see below), you need to compile Boost.Test into a dynamic library and point your linker to its location. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_54_0/more/getting_started/index.html) on how to do this on your system.

* **NOTE:** compilation of the Boost libraries is **ONLY** a requirement for running the test-suite, and **NOT** for using the DCTL headers with your application.  

### CMake

The test-suite is built using the [CMake](http://www.cmake.org/) cross-platform build system. Most development environments can run the CMake generated Makefiles. To generate native build solutions for your own development environment, consult the [CMake documentation](http://www.cmake.org/cmake/help/runningcmake.html).

* **NOTE:** CMake is **ONLY** a requirement for building the test-suite, and **NOT** for using the DCTL headers with your application. 

### Mercurial

Development is being tracked with the [Mercurial](http://mercurial.selenic.com/) distributed version control system, the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) extension, and the [BitBucket](https://bitbucket.org) hosting service. The default branch is the currently stable version and the commits along this branch are tagged with versions. Development takes place on the develop branch, with features being developed in so-called feature branches.

* **NOTE**: Mercurial is **ONLY** a requirement for contributing to the DCTL, and **NOT** for using the DCTL headers with your application. Downloading and unpacking the latest zipped source archive works equally well.

Installation
------------

### Download

Clone this repository to a directory of your choice 

      cd ~/projects/ 
      hg clone https://bitbucket.org/rhalbersma/dctl/

Alternatively, you can download and unpack the latest zipped source archive into the same directory.

### Test-suite

To make sure that your build environment is compatible with the DCTL requirements, the DCTL comes with an extensive suite of unit tests, placed in the `dctl/test` sub-directory. To build and run the test-suite, follow the requirements (in particular: compile Boost), and type

      cd ~/projects/dctl/test    
      mkdir build
      cd build
      cmake ..
      make -j10
      ctest -j10 -E "walk|search"

The build will take about half a minute on a 3.2 GHz Intel i7 (and longer for systems with less parallelism). The test-suite itself takes a fraction of second to run. Note that the `ctest` command excludes all unit tests that do a tree walk or tree search (these tests will take several minutes to hours to run, respectively).

To completely regenerate the test-suite's build solution, simply delete the contents of the entire `build/` directory and rerun the above commands. To skip the `cmake` configuration step, and only rebuild and rerun the test-suite, simply type 

      make clean
      make -j10
      ctest -j10 -E "walk|search" 

The test-suite runner follows the [UNIX rule of silence](http://www.linfo.org/rule_of_silence.html): if you do not see any errors, the tests succeeded. Congratulations: your system supports the DCTL, and you are now ready to start coding!

* **NOTE**: your application is completely independent of the test-suite. Building and running the test-suite is a sufficient but not a necessary step in order to use the DCTL headers with your own application. 

### Build your application

The DCTL is header-only, which means that you do not have to link your application against a separately compiled library. Furthermore, even though the DCTL is dependent on Boost, you do not have to separately compile Boost yourself, or even `#include` any Boost headers. Simply point your compiler to the location of both the DCTL and the Boost headers, `#include` the appropriate DCTL header files into your application, and then continue to use your regular build process. 

### Contribute

Any feature requests, ideas and contributions are much appreciated! The recommended way to contribute code is through BitBucket [pull requests](https://confluence.atlassian.com/display/BITBUCKET/Working+with+pull+requests).    

Acknowledgments
----------------

Special thanks to Ed Gilbert, Aart Bik, Wieger Wesselink and Walter Thoen for encouragement, testing, and exchange of ideas.

License
-------

Copyright Rein Halbersma 2010 - 2013.   
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/users/license.html).   
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
	
	Boost Software License - Version 1.0 - August 17th, 2003

	Permission is hereby granted, free of charge, to any person or organization
	obtaining a copy of the software and accompanying documentation covered by
	this license (the "Software") to use, reproduce, display, distribute,
	execute, and transmit the Software, and to prepare derivative works of the
	Software, and to permit third-parties to whom the Software is furnished to
	do so, all subject to the following:

	The copyright notices in the Software and this entire statement, including
	the above license grant, this restriction and the following disclaimer,
	must be included in all copies of the Software, in whole or in part, and
	all derivative works of the Software, unless such copies or derivative
	works are solely in the form of machine-executable object code generated by
	a source language processor.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.

