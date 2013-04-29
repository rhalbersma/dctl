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

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. It is currently not possible to play a game against a program based on the DCTL. Support for such functionality is being planned. 

Requirements
============

### C++11 compiler

The DCTL is a modern [C++](isocpp.org) library that is dependent on many of the features that have come available with the new [C++11 Standard](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3376.pdf). Compiler support for C++11 differs across compilers. See this [site](http://wiki.apache.org/stdcxx/C%2B%2B0xCompilerSupport) for an up-to-date overview of C++11 support by all major compiler vendors. 

The C++11 features that the DCTL currently depends on consist of `auto`, initializer lists, lambda, `nullptr`, range-based for-loop, r-value references, `static_assert`, and variadic templates. The DCTL is exclusively being developed on Linux with g++ >= 4.7.2, but the Intel >= 13.0 and Clang >= 3.1 compilers should also work (untested). The latest Microsoft Visual C++ compiler (2012 November CTP) has insufficient support to reliably compile the current testsuite, and is currently unsupported (subject to change when new updates come available).

### Boost

The DCTL also uses the [Boost C++ libraries](www.boost.org) in many places. Examples are  Boost.MPL, Boost.Operators, and Boost.Preprocessor. The unit testing framework is based on Boost.Test. The DCTL is being developed with Boost >= 1.53.0, but any version >= 1.47.0 should also work (untested). See the [documentation](http://www.boost.org/doc/libs/1_53_0/more/getting_started/) on how to install Boost on your system.

### Platforms

The DCTL is being developed under Linux Mint 14.1 64-bit. The code should also compile on Apple OSX with the Clang >= 3.1 compiler (untested). Support for Android (using the Android NDK to interface the Java GUI) is being considered. For Microsoft Windows development, consider switching to a recent version of the [MinGW gcc compiler](http://nuwen.net/mingw.html).

Installation
============

### Build environment

The DCTL development is being tracked with the [Mercurial](http://hginit.com/) distribed version control system and the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) extension. The default branch is the currently stable version and the commits along this branch are tagged with versions. Development takes place on the develop branch, with several features being developed in parallel in so-called feature branches.

### Download

Clone this BitBucket repository to a directory of your choice

      cd projects/
      hg clone https://bitbucket.org/rhalbersma/dctl/

### Header-only

The DCTL is header-only, which means that you do not have link your application against a seperately compiled library. That's right: no DLL hell. Furthermore, even though the DCTL is dependent on Boost, you do not have to separately compile Boost yourself, or even #include any Boost headers. Simply point your compiler to the location of both the DCTL and the Boost headers, #include the appropriate DCTL header files into your application, and then continue to use your regular build process. 

### Testing

To make sure that your build environment is compatible with the DCTL requirements, the DCTL comes with an extensive suite of unit tests, placed in the dctl/test subdirectory. To build and run them, simply type the following five commands
    
    mkdir build
    cd build
    cmake ..
    make
    make test

NOTE: in order to build and run the tests, you have to compile Boost.Test into a static library. Consult the [Boost Documentation](http://www.boost.org/doc/libs/1_53_0/more/getting_started/index.html) for more information.

The unit tests build target does not have a `make clean` command. You can simply delete the entire build directory and run the above commands again (e.g. after you changed something in your own build system). The use of the DCTL library in your application is completely independent of the library's own unit tests.

Roadmap
=======

The DCTL is actively being developed with over 600 commits in the last 3 years. Using the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) workflow manager, several features will be developed in parallel. The main priority is to enhance the genericity of the search algorithms so that users can more easily experiment with their own engines. Other functionality being planned include:   
-CMake support for cross-platform builds   
-a tree iterator   
-a generalized bit array to support arbitrarily large boards   
-an engine class   
-support for the Checkerboard GUI   
-support for the GUIDE protocol   
-support for the DamExchange protocol  
-endgame database generation   

Most of these features are being tracked in so-called feature branches, and will be merged back into the main tree when they are finished.

Acknowledgements
================

A big thanks to everyone who has been supportive over the years, either by downloading and compiling the code, or by hearing me out when I wanted to bounce and idea off someone. Special thanks to Ed Gilbert, Aart Bik and Wieger Wesselink for their efforts in testing.

License
=======

Copyright Rein Halbersma (rhalbersma@gmail.com) 2010 - 2013.   
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