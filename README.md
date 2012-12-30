DCTL: Draughts and Checkers Template Library
============================================

A [C++](isocpp.org) template library for the games of [draughts](http://en.wikipedia.org/wiki/Draughts) and [checkers](http://en.wikipedia.org/wiki/Checkers_(disambiguation)), supporting all known rules variations, board sizes and game objectives.

Introduction
============

Draughts is a family of related board games played in many regions across the world. Each country has its own game rules and board size. Most of the draughts variants have official game rules and a governing body, such as the [Fédération Mondiale du Jeu de Dames](fmjd.org) (FMJD, World Draughts Federation). These variants include International/Brazilian/Canadian, American/English, Russian, Italian, Spanish/Portugese, Czech, Thai, and Frisian draughts. These games are commonly played on checkered boards of dimensions 8x8, 10x10 and 12x12. The common game objectives is "the first player to have no moves left, loses". A commonly played variation is so-called misère play ("first to have no moves left, wins"), also known as "suicide" draughts (or "qui-perd-gagné"). 

Scope
=====

The DCTL supports all above mentioned draughts variants with official rules, on all board sizes smaller than 12x12. Support for 12x12 boards is tentatively planned. Because draughts has a high percentage of draws when played between experienced players, many variants with small tweaks to the game rules have been proposed in the past. Even world champions such as Herman Hoogland proposed such new game variants. The DCTL also supports some of these games. In particular, the recently proposed game of [Killer Draughts](http://www.killerdraughts.org/) is actively being supported.

The DCTL provides a series of C++ templates to define the board geometry and game rules of draughts variants. With these templates, one can read and write draughts positions from so-called FEN strings using [Portable Draughts Notation](http://www.10x10.dse.nl/pdn/introduction.html). Once a position variable has been initalized with such a FEN string, the DCTL provides template functions to analyze them using a search algorithm, which is supported by the usual infrastructure of a move generator, evaluation function and hash table.

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. Support for such functionality is being planned. 

Requirements
============

### C++11

The DCTL is a modern [C++](isocpp.org) library that is dependent on many of the features that have come available with the new C++11 Standard. Compiler support for C++11 differs across compilers. See this [site](http://wiki.apache.org/stdcxx/C%2B%2B0xCompilerSupport) for an up-to-date overview of C++11 support by all major compiler vendors. The C++11 features that the DCTL currently depends on consist of auto, initializer lists, lambda, nullptr, range-based for-loop, r-value references, static_assert, and variadic templates. The DCTL is being developed with g++ >= 4.6.3 and with Microsoft Visual C++ >= 11.0 (November 2012 CTP), but the Intel >= 13.0 and Clang >= 3.1 compilers should also work (untested). As soon as support for more features also becomes available (such as constexpr, default/deleted functions, and template aliases) in Microsoft Visual C++, the DCTL will aggressively adopt such features.

### Boost

The DCTL also uses the [Boost C++ libraries](www.boost.org) in many places. Examples are Boost.Algorithm, Boost.Assert, Boost.Lexical_Cast, Boost.MPL, Boost.Operators, Boost.Preprocessor, and Boost.Utility. The unit testing framework is based on Boost.Test. The DCTL is being developed with Boost >= 1.52.0, but any version >= 1.47.0 should also work (untested). See the [documentation](http://www.boost.org/doc/libs/1_52_0/more/getting_started/) on how to install Boost on your system.

### Platforms

The DCTL is being developed under both Microsoft Windows 7 64-bit and Linux Mint 14.1 64-bit. The code should also compile on Apple OSX with the Clang >= 3.1 compiler (untested). Support for Android (using the Android NDK to interface the Java GUI) is being considered. NOTE: there is a [critical patch](http://laatste.info/bb3/viewtopic.php?f=53&t=3910&start=14) required to build with Microsoft Visual C++ 2012, and even then the build will only succeed in Release mode, but not in Debug mode.

Installation
============

### Build environment

Apart from a modern C++ compiler and the Boost libraries, it is convenient to have a modern build environment. The DCTL is being developed under Microsoft Visual Studio (with the Microsoft Visual C++ compiler) and Eclipse (with both the MinGW and Linux gcc compilers). The necessary build files can be found in the dctl/build subdirectory. It is also possible to generate a makefile from within Eclipse. Support for the cross-build platform [CMake](http://cmake.org/) is being considered. The DCTL development is being tracked with the [Mercurial](http://hginit.com/) distribed version control system and the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) extension. The default branch is the currently stable version and the commits along this branch are tagged with versions. Development takes place on the develop branch, with several features being developed in parallel in so-called feature branches.

### Download

Either clone this BitBucket repository or download a .zip or tar ball. Unpack the entire source tree into a directory of choice. Typically, users need to make their build environments aware of the paths to the DCTL source and the Boost libraries. Consult your build system's documentation for more details. Note that Boost can be installed in a directory that is automatically being tracked by your build environment. Consult the Boost documentation for more details.

### Header-only

The DCTL is header-only, so simply #including the appropriate header files should let your application build successfully.

### Unit-testing

The DCTL comes with an extensive suite of unit tests, placed in the dctl/test subdirectory. You need to statically link these files agains the boost_unit_test_framework library.  NOTE: the Microsoft Windows platform supports auto-linking that will automatically detect the presence of static and dynamic libraries. It is convenient to run the test binary as a post-build event so that the build succeeds if and only if the unit tests succeed. Consult your build environment's documentation for more details. Some of the unit tests take a long time (they are actually integration tests) to run. These have been disabled by default in the test/config.hpp file.

Roadmap
=======

The DCTL is actively being developed with over 500 commits in the last 2 years. Using the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) workflow manager, several features will be developed in parallel. The main priority is to enhance the genericity of the search algorithms so that users can more easily experiment with their own engines. Other functionality being planned include:
-CMake support for cross-platform builds
-a tree iterator
-a generalized bit array to support arbitrarily large boards
-an engine class
-support for the Checkerboard GUI
-support for the GUIDE protocol
-support for the DamExchange protocol
-endgame database generation

Most of these features are being tracked in so-called feature branches, and will be merged back into the main tree when they are finished.

License
=======

Copyright Rein Halbersma (rhalbersma@gmail.com) 2010 - 2012.   
Distributed under the Boost Software License, Version 1.0.   
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