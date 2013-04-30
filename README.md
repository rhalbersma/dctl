DCTL: Draughts and Checkers Template Library
============================================

A [C++](http://isocpp.org) template library for the games of [draughts](http://en.wikipedia.org/wiki/Draughts) and [checkers](http://en.wikipedia.org/wiki/Checkers_(disambiguation)), supporting almost all known rules variations, board sizes and game objectives.

Rationale
---------

Draughts is a family of related board games played in many regions across the world, with many country-specific game rules and board size. Many draughts variants have an official governing body, such as the [Fédération Mondiale du Jeu de Dames](htpp://fmjd.org) (FMJD, World Draughts Federation). These variants include International/Brazilian/Canadian, American/English, Russian, Italian, Spanish/Portugese, Czech, Thai, and Frisian draughts. These games are commonly played on checkered boards of dimensions 8x8, 10x10 and 12x12. The common game objectives is "the first player to have no moves left, **loses**". A less common variation is so-called misère play ("first to have no moves left, **wins**"), also known as "suicide" draughts (or "qui-perd-gagné"). 

Scope
-----

The DCTL supports all above mentioned draughts variants with official rules, on all board sizes smaller than 12x12. Support for 12x12 boards is tentatively planned. Because draughts has a high percentage of draws when played between experienced players, many variants with small tweaks to the game rules have been proposed in the past. Even former world champions such as [Herman Hoogland](http://en.wikipedia.org/wiki/Herman_Hoogland) proposed such new game variants. The DCTL also supports some of these variants. In particular, the recently proposed [Killer Draughts](http://www.killerdraughts.org/) is actively supported.

The DCTL provides a series of C++ templates to define the board geometry and game rules of draughts variants. With these templates, one can read and write draughts positions from so-called FEN strings using [Portable Draughts Notation](http://www.10x10.dse.nl/pdn/introduction.html). Once a position variable has been initalized with such a FEN string, the DCTL provides template functions to analyze them using a search algorithm, which is supported by the usual infrastructure of a move generator, evaluation function and hash table.

Roadmap
-------

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. It is currently not possible to play a game against a program based on the DCTL. Functionality being planned includes:   

* a generic engine class template   
* support for the [Checkerboard GUI](http://www.fierz.ch/cbdeveloper.php)   
* support for the [GUIDE protocol]()   
* support for the [DamExchange protocol](http://www.mesander.nl/damexchange/edxpmain.htm)  
* endgame database generation   

Requirements
------------

### Platforms

The DCTL is a cross-platform library

* **Supported:** current development takes place on Linux Mint 14.1 64-bit and Microsoft Windows 7 64-bit.
* **Untested:**  successful builds should be possible for the Google Android platform (using the [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html)). This is currently untested.
* **Unsupported:** successful builds should be possible for the Apple Mac OS X and iOS platforms. This is currently supported.

### Compilers

The DCTL is a modern [C++](http://isocpp.org) library that is dependent on many of the features that have come available with the new [C++11 Standard](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3376.pdf). The C++11 features that the DCTL currently depends on consist of `auto`, initializer lists, lambda expressions, `nullptr`, range-based `for`-loop, rvalue references, `static_assert`, and variadic templates. Unfortunately, feature support for C++11 [differs across compiler vendors](http://wiki.apache.org/stdcxx/C%2B%2B0xCompilerSupport). 

* **Supported:** the only supported compiler is [g++](http://gcc.gnu.org/projects/cxx0x.html). Current development takes place with g++ 4.7.2 (on Linux) and MinGW g++ 4.8.0 (on Windows), and the minimum required version is 4.6.3 (untested).
* **Untested:** recent versions of either the [Intel](http://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler) (version 13.0 or higher) or [Clang](http://clang.llvm.org/cxx_status.html) (version 3.1 or higher) compilers should correctly build the DCTL testsuite, but either are currently untested.
* **Unsupported:** the [Microsoft Visual C++ 2012](http://msdn.microsoft.com/en-us/library/vstudio/hh567368.aspx) compiler, even with its [November CTP](http://blogs.msdn.com/b/vcblog/archive/2012/11/02/visual-c-c-11-and-the-future-of-c.aspx), currently has insufficient(ly stable) C++11 support to reliably compile the DCTL testsuite, and is currently unsupported (subject to change when new updates come available).

### Libraries

The DCTL heavily uses the [Boost C++ libraries](http://www.boost.org) in many places. Examples are Boost.Accumulators, Boost.MPL, Boost.Operators, and Boost.Preprocessor. The unit testing framework is based on Boost.Test. Current development takes place with Boost 1.53.0, and the minimum required version is 1.49.0 (untested).

Installation
------------

### Build environment

The DCTL development is being tracked with the [Mercurial](http://hginit.com/) distribed version control system and the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) extension. The default branch is the currently stable version and the commits along this branch are tagged with versions. Development takes place on the develop branch, with features being developed in so-called feature branches.

### Download

Clone this BitBucket repository to a directory of your choice 

      cd projects/ 
      hg clone https://bitbucket.org/rhalbersma/dctl/

### Header-only

The DCTL is header-only, which means that you do not have link your application against a seperately compiled library. That's right: no DLL hell. Furthermore, even though the DCTL is dependent on Boost, you do not have to separately compile Boost yourself, or even #include any Boost headers. Simply point your compiler to the location of both the DCTL and the Boost headers, #include the appropriate DCTL header files into your application, and then continue to use your own build process. 

**NOTE**: in order to build and run the testsuite (see below), you have to compile Boost.Test into a static library and point your linker to its location. Consult the [Boost Documentation](http://www.boost.org/doc/libs/1_53_0/more/getting_started/index.html) for more information. Most IDEs can run the CMake generated Makefiles. To generate native build solutions for your own environment, consult the [CMake documentation](http://www.cmake.org/cmake/help/runningcmake.html).

### Testing

To make sure that your build environment is compatible with the DCTL requirements, the DCTL comes with an extensive suite of unit tests, placed in the dctl/test subdirectory. To build and run them, you need to install [CMake](http://www.cmake.org/) (version 2.8 or higher) and type the following five commands inside the `dctl/` root directory
    
      mkdir build
      cd build
      cmake ..
      make
      make test

**NOTE**: our application is completely independent of the library's own unit tests. To completely regenerate the testsuite's build solution, simply delete the entire `dctl/build/` directory and rerun the above five commands. To only rebuild and rerun the testsuite, type

      make clean
      make
      make test

Acknowledgements
----------------

A big thanks to everyone who has been supportive over the years, either by downloading and compiling the code, or by hearing me out when I wanted to bounce and idea off someone. Special thanks to Ed Gilbert, Aart Bik and Wieger Wesselink for their efforts in testing.

License
-------

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

