DCTL: Draughts and Checkers Template Library
============================================

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/) 
[![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) 
[![License](https://img.shields.io/badge/license-Boost-blue.svg)](https://opensource.org/licenses/BSL-1.0) 
[![Build Status](https://img.shields.io/travis/rhalbersma/xstd.svg)](https://travis-ci.org/rhalbersma/xstd)

A [C++](http://isocpp.org) template library for the games of [draughts](http://en.wikipedia.org/wiki/Draughts) and [checkers](http://en.wikipedia.org/wiki/Checkers_(disambiguation)), supporting almost all known rules variations, board sizes and game objectives.

* **DISCLAIMER:** This library is currently in a [pre-alpha](https://en.wikipedia.org/wiki/Software_release_life_cycle#Pre-alpha) stage. All code is subject to change without prior notice. 

Introduction
------------

Draughts is a family of related board games played in many [regions](http://alemanni.pagesperso-orange.fr/geography.html) across the world. There are many country-specific game rules such as International/Brazilian/Canadian, American/English, Russian, Italian, Spanish/Portugese, Czech, Thai, and Frisian draughts. Draughts is played on checkered boards of dimensions 8x8, 10x10 and 12x12. The common game objective is "the first player to have no moves left, **loses**". A less common variation is so-called misère play ("first to have no moves left, **wins**"), also known as "suicide", "giveaway", or "qui-perd-gagné". Many draughts variants have an official governing body, such as the [Fédération Mondiale du Jeu de Dames](htpp://fmjd.org) (FMJD, World Draughts Federation).

Scope
-----

The DCTL supports all above mentioned draughts variants with official rules, on all board sizes up to 18x18. Most existing draughts variants have a high percentage of draws when played between experienced players. Historically, many small tweaks to the game rules have been proposed. Even former world champions such as [Herman Hoogland](http://en.wikipedia.org/wiki/Herman_Hoogland) proposed new variants. The DCTL supports some of these variants. In particular, the recently proposed [Killer Draughts](http://www.killerdraughts.org/) is actively supported.

Usage
-----

The DCTL provides a series of [C++ templates](http://en.wikipedia.org/wiki/Template_(C%2B%2B)) to define the board geometry and game rules of draughts variants. With these templates, one can read and write draughts positions from so-called FEN strings using [Portable Draughts Notation](http://www.10x10.dse.nl/pdn/introduction.html). Once a position variable has been initialized with such a FEN string, the DCTL provides further templates to analyze them using a search algorithm, which is supported by a typical board game [engine infrastructure](http://chessprogramming.wikispaces.com/) of e.g. a move generator, evaluation function and hash table.

* **TIP**: Consult the test-suite sources (under the `dctl/test` directory) for concrete usage examples.

Road-map
-------

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. It is currently not possible to play a game against a program based on the DCTL. Functionality being planned includes:   

* a generic engine class template   
* support for the [Checkerboard GUI](http://www.fierz.ch/cbdeveloper.php)   
* support for the [GUIDE protocol](http://laatste.info/bb3/download/file.php?id=864)   
* support for the [DamExchange protocol](http://www.mesander.nl/damexchange/edxpmain.htm)  
* endgame database generation   

Requirements
------------

##### Platforms

The DCTL aims to be cross-platform in the near future, but is currently only supported on .deb based 64-bit Linux distributions (Mint, Ubuntu, Debian). Ubuntu-based distributions 16.04 LTS and higher are actively supported.

##### Compilers

The DCTL is a modern [C++](http://isocpp.org) library that targets the upcoming [C++17 Standard](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf). This currently restricts usage of the DCTL to either [gcc](https://gcc.gnu.org/projects/cxx-status.html), version 7.1 or higher, or [Clang](http://clang.llvm.org/cxx_status.html) version 5.0 or higher. Visual C++ is not yet supported because of its currently incomplete C++17 support. Stay tuned for further developments.

##### Boost headers

The DCTL uses several of the popular [Boost C++ libraries](http://www.boost.org). Current development takes place with Boost 1.64.0. Boost is a collection of header-only libraries, and you simply have to point your compiler to the Boost include directory. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_64_0/more/getting_started/index.html) on how to do this on your system. After that, you can continue to use your regular build process.

##### Boost libraries

The test-suite uses [Boost.Test](http://www.boost.org/doc/libs/1_64_0/libs/test/doc/html/index.html). In order to build and run the test-suite (see below), you need to compile Boost.Test into a dynamic library and point your linker to its location. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_64_0/more/getting_started/index.html) on how to do this on your system.

* **NOTE:** compilation of the Boost libraries is **ONLY** a requirement for running the test-suite, and **NOT** for using the DCTL headers with your application.  

##### CMake

The test-suite is built using the [CMake](http://www.cmake.org/) cross-platform build system. Most Linux development environments can directly access the CMake generated Makefiles. This has been tested with [Eclipse-CDT 4.4](http://www.vtk.org/Wiki/CMake:Eclipse_UNIX_Tutorial) and [QtCreator 3.0](http://qt-project.org/doc/qtcreator-3.0/creator-project-cmake.html). Instead of `make`, it is also possible to use the `ninja` build tool through `cmake -GNinja`. To generate native build solutions for your own development environment, consult the [CMake documentation](http://www.cmake.org/cmake/help/runningcmake.html).

* **NOTE:** CMake is **ONLY** a requirement for building the test-suite, and **NOT** for using the DCTL headers with your application. 

Installation
------------

##### Download

Clone the `dctl` and the accompanying `xstd` repository (containing C++ Standard Library extensions headers) to a directory of your choice 

      cd ~/projects/ 
      git clone https://github.com/rhalbersma/dctl.git
      git clone https://github.com/rhalbersma/xstd.git

##### Test-suite

To make sure that your build environment is compatible with the DCTL requirements, the DCTL comes with an extensive suite of unit tests, placed in the `dctl/test` sub-directory. To build and run the test-suite, follow the requirements (in particular: compile Boost), and type

      cd ~/projects/dctl/    
      mkdir build
      cd build
      cmake ..
      make -j10
      ctest -j10 -E "search|traversal"

The build will take less than a minute on a 3.5 GHz Intel i7 (and longer for systems with less parallelism). The test-suite itself takes a second to run. Note that the `ctest` command excludes all unit tests that do a tree search or traversal (these tests will take several minutes to hours to run, respectively).

To completely regenerate the test-suite's build solution, simply delete the contents of the entire `build/` directory and rerun the above commands. To skip the `cmake` configuration step, and only rebuild and rerun the test-suite, simply type 

      make clean
      make -j10
      ctest -j10 -E "search|traversal" 

If you do not see any errors, the tests succeeded. Congratulations: your system supports the DCTL, and you are now ready to start coding!

* **NOTE**: your application is completely independent of the test-suite. Building and running the test-suite is a sufficient but not a necessary step in order to use the DCTL headers with your own application. 

##### Build your application

The DCTL is header-only, which means that you do not have to link your application against a separately compiled library. Furthermore, even though the DCTL is dependent on Boost, you do not have to separately compile Boost yourself, or even `#include` any Boost headers. Simply point your compiler to the location of both the DCTL and the Boost headers, `#include` the appropriate DCTL header files into your application, and then continue to use your regular build process. 

##### Contribute

Any feature requests, ideas and contributions are much appreciated! The recommended way to contribute code is through GitHub pull requests.

##### Acknowledgments

Special thanks to Aart Bik, Ed Gilbert, Walter Thoen and Wieger Wesselink for encouragement, testing, and exchange of ideas.

License
-------

Copyright Rein Halbersma 2010 - 2017.   
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/users/license.html).   
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
