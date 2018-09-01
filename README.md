DCTL: Draughts and Checkers Template Library
============================================

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/) 
[![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) 
[![License](https://img.shields.io/badge/license-Boost-blue.svg)](https://opensource.org/licenses/BSL-1.0)
[![](https://tokei.rs/b1/github/rhalbersma/dctl)](https://github.com/rhalbersma/dctl)

A [C++](http://isocpp.org) template library for the games of [draughts](http://en.wikipedia.org/wiki/Draughts) and [checkers](http://en.wikipedia.org/wiki/Checkers_(disambiguation)), supporting almost all known rules variations, board sizes and game objectives.

> #### Disclaimer
> This library is currently in a [pre-alpha](https://en.wikipedia.org/wiki/Software_release_life_cycle#Pre-alpha) stage. All code is subject to change without prior notice. 

Introduction
------------

Draughts is a family of related board games played in many [regions](http://alemanni.pagesperso-orange.fr/geography.html) across the world:

* There are many country-specific game rules such as International/Brazilian/Canadian, American/English, Russian, Italian, Spanish/Portugese, Czech, Thai, and Frisian draughts. 
* Draughts is played on checkered boards of dimensions 8x8, 10x10 and 12x12. 
* The common game objective is "the first player to have no moves left, **loses**". A less common variation is so-called misère play ("first to have no moves left, **wins**"), also known as "suicide", "giveaway", or "qui-perd-gagné". 

Many draughts variants have an official governing body, such as the [Fédération Mondiale du Jeu de Dames](htpp://fmjd.org) (FMJD, World Draughts Federation).

Scope
-----

The DCTL supports all above mentioned draughts variants with official rules, on all board sizes up to 18x18. Most existing draughts variants have a high percentage of draws when played between experienced players. Historically, many small tweaks to the game rules have been proposed. Even former world champions such as [Herman Hoogland](http://en.wikipedia.org/wiki/Herman_Hoogland) proposed new variants. The DCTL supports some of these variants. In particular, the recently proposed [Killer Draughts](http://www.killerdraughts.org/) is actively supported.

Usage
-----

The DCTL provides a series of [C++ templates](http://en.wikipedia.org/wiki/Template_(C%2B%2B)) to define the board geometry and game rules of draughts variants. With these templates, one can read and write draughts positions from so-called FEN strings using [Portable Draughts Notation](http://www.10x10.dse.nl/pdn/introduction.html). Once a position variable has been initialized with such a FEN string, the DCTL provides further templates to analyze them using a search algorithm, which is supported by a typical board game [engine infrastructure](http://chessprogramming.wikispaces.com/) of e.g. a move generator, evaluation function and hash table.

> #### Tip 
> Consult the test-suite sources (under the `dctl/test` directory) for concrete usage examples.

Road-map
-------

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. It is currently not possible to play a game against a program based on the DCTL. Functionality being planned includes:   

- [ ] a generic engine class template
- [ ] support for the [Checkerboard GUI](http://www.fierz.ch/cbdeveloper.php)
- [ ] support for the [GUIDE protocol](http://laatste.info/bb3/download/file.php?id=864)   
- [ ] support for the [DamExchange protocol](http://www.mesander.nl/damexchange/edxpmain.htm)
- [ ] endgame database generation   

Requirements
------------

These header-only libraries are continuously being tested with the following conforming [C++17](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf) compilers:

| Platform | Compiler | Versions | Build |
| :------- | :------- | :------- | :---- |
| Linux    | Clang <br> GCC | 6.0, 7-SVN<br> 8.1 | [![codecov](https://codecov.io/gh/rhalbersma/dctl/branch/master/graph/badge.svg)](https://codecov.io/gh/rhalbersma/xstd) <br> [![Build Status](https://travis-ci.org/rhalbersma/dctl.svg)](https://travis-ci.org/rhalbersma/dctl) |
| Windows  | Visual Studio  |                    15.7 | [![Build status](https://ci.appveyor.com/api/projects/status/3pf9sx7giutm2wtx?svg=true)](https://ci.appveyor.com/project/rhalbersma/dctl) |

##### Boost library

The DCTL uses several of the popular [Boost C++ libraries](http://www.boost.org). Current development takes place with Boost 1.66.0. Boost is a collection of header-only libraries, and you simply have to point your compiler to the Boost include directory. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_66_0/more/getting_started/index.html) on how to do this on your system. After that, you can continue to use your regular build process.

Installation
------------

##### Download

Clone the `dctl` and the accompanying `xstd` and `int_set` repositories to a directory of your choice:

      cd ~/projects/ 
      git clone https://github.com/rhalbersma/dctl.git
      git clone https://github.com/rhalbersma/xstd.git
      git clone https://github.com/rhalbersma/int_set.git

##### Test-suite

To make sure that your build environment is compatible with the DCTL requirements, the DCTL comes with an extensive suite of unit tests, placed in the `dctl/test` sub-directory. To build and run the test-suite, follow the requirements (in particular: compile Boost.Test), and from inside the directory in which you downloaded the DCTL, type (where `-jN` builds on `N` cores in parallel)

      mkdir build && cd build
      cmake ..
      cmake --build . -- -jN
      ctest -E "search|traversal"

The build will take less than a minute on a 3.5 GHz Intel i7 (and longer for systems with less parallelism). The test-suite itself takes a second to run. Note that the `ctest` command excludes all unit tests that do a tree search or traversal (these tests will take several minutes to hours to run, respectively). If you do not see any errors, the tests succeeded. Congratulations: your system supports the DCTL, and you are now ready to start coding!

> #### Note 
> Your application is completely independent of the test-suite. Building and running the test-suite is a sufficient but not a necessary step in order to use the DCTL headers with your own application. 

##### Build your application

The DCTL is header-only, which means that you do not have to link your application against a separately compiled library. Furthermore, even though the DCTL is dependent on Boost, you do not have to separately compile Boost yourself, or even `#include` any Boost headers. Simply point your compiler to the location of both the DCTL and the Boost headers, `#include` the appropriate DCTL header files into your application, and then continue to use your regular build process. 

##### Contribute

Any feature requests, ideas and contributions are much appreciated! The recommended way to contribute code is through GitHub pull requests.

##### Acknowledgments

Special thanks to Aart Bik, Ed Gilbert, Walter Thoen and Wieger Wesselink for encouragement, testing, and exchange of ideas.

License
-------

Copyright Rein Halbersma 2010 - 2018.   
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/users/license.html).   
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
