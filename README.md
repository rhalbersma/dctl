# DCTL: Draughts and Checkers Template Library

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-23-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-Boost-blue.svg)](https://opensource.org/licenses/BSL-1.0)
[![Lines of Code](https://tokei.rs/b1/github/rhalbersma/dctl?category=code)](https://github.com/rhalbersma/dctl)

A [C++](http://isocpp.org) template library for the games of [draughts](http://en.wikipedia.org/wiki/Draughts) and [checkers](http://en.wikipedia.org/wiki/Checkers_(disambiguation)), supporting almost all known rules variations, board sizes and game objectives.

## Disclaimer

> This library is currently in a [pre-alpha](https://en.wikipedia.org/wiki/Software_release_life_cycle#Pre-alpha) stage. All code is subject to change without prior notice.

## Introduction

Draughts is a family of related board games played in many [regions](http://alemanni.pagesperso-orange.fr/geography.html) across the world:

* There are many country-specific game rules such as International/Brazilian/Canadian, American/English, Russian, Italian, Spanish/Portugese, Czech, Thai, and Frisian draughts.
* Draughts is played on checkered boards of dimensions 8x8, 10x10 and 12x12.
* The common game objective is "the first player to have no moves left, **loses**". A less common variation is so-called misère play ("first to have no moves left, **wins**"), also known as "suicide", "giveaway", or "qui-perd-gagné".

Many draughts variants have an official governing body, such as the [Fédération Mondiale du Jeu de Dames](htpp://fmjd.org) (FMJD, World Draughts Federation).

## Scope

The DCTL supports all above mentioned draughts variants with official rules, on all board sizes up to 18x18. Most existing draughts variants have a high percentage of draws when played between experienced players. Historically, many small tweaks to the game rules have been proposed. Even former world champions such as [Herman Hoogland](http://en.wikipedia.org/wiki/Herman_Hoogland) proposed new variants. The DCTL supports some of these variants. In particular, the recently proposed [Killer Draughts](http://www.killerdraughts.org/) is actively supported.

## Usage

The DCTL provides a series of [C++ templates](http://en.wikipedia.org/wiki/Template_(C%2B%2B)) to define the board geometry and game rules of draughts variants. With these templates, one can read and write draughts positions from so-called FEN strings using [Portable Draughts Notation](http://www.10x10.dse.nl/pdn/introduction.html). Once a position variable has been initialized with such a FEN string, the DCTL provides further templates to analyze them using a search algorithm, which is supported by a typical board game [engine infrastructure](http://chessprogramming.wikispaces.com/) of e.g. a move generator, evaluation function and hash table.

## Tip

> Consult the test-suite sources (under the `dctl/test` directory) for concrete usage examples.

## Road-map

The DCTL does not yet provide a fully functioning game engine that can be plugged into a GUI. It is currently not possible to play a game against a program based on the DCTL. Functionality being planned includes:

* [ ] a generic engine class template
* [ ] support for the [Checkerboard GUI](http://www.fierz.ch/cbdeveloper.php)
* [ ] support for the [GUIDE protocol](http://damforum.nl/bb3/download/file.php?id=864)
* [ ] support for the [DamExchange protocol](http://www.mesander.nl/damexchange/edxpmain.htm)
* [ ] endgame database generation

## Requirements

These header-only libraries depend on the C++ Standard Library and [Boost](http://boost.org) and are continuously being tested with the following conforming conforming [C++23](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf) compilers:

| Platform | Compiler   | Versions     | Build |
| :------- | :-------   | -------:     | :---- |
| Linux    | GCC        | 14, 15-trunk | CI currently being ported to GitHub Actions |
| Linux    | Clang      | 18, 19-trunk | CI currently being ported to GitHub Actions |
| Windows  | Visual C++ | 17.10        | CI currently being ported to GitHub Actions |

## Installation

### Download

Clone the `dctl` and the `bit_set`, `tabula` and `xstd` repositories as dependencies to a directory of your choice:

      cd ~/projects/
      git clone https://github.com/rhalbersma/dctl.git
      git clone https://github.com/rhalbersma/bit_set.git
      git clone https://github.com/rhalbersma/tabula.git
      git clone https://github.com/rhalbersma/xstd.git

### Test-suite

To make sure that your build environment is compatible with the DCTL requirements, the DCTL comes with an extensive suite of unit tests, placed in the `dctl/test` sub-directory. To build and run the test-suite, follow the requirements (in particular: compile Boost.Test), and type (where `-j N` builds on `N` cores in parallel)

      cd dctl
      mkdir build && cd build
      cmake ..
      cmake --build . -j N
      ctest -E "search|traversal"

The build will take less than a minute on a 3.5 GHz Intel i7 (and longer for systems with less parallelism). The test-suite itself takes a second to run. Note that the `ctest` command excludes all unit tests that do a tree search or traversal (these tests will take several minutes to hours to run, respectively). If you do not see any errors, the tests succeeded. Congratulations: your system supports the DCTL, and you are now ready to start coding!

## Contribute

Any feature requests, ideas and contributions are much appreciated! The recommended way to contribute code is through GitHub pull requests.

## Acknowledgments

Special thanks to Aart Bik, Ed Gilbert, Fabien Letouzey, Walter Thoen and Wieger Wesselink for encouragement, testing, and exchange of ideas.

## License

Copyright Rein Halbersma 2010-2023.
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/users/license.html).
(See accompanying file LICENSE_1_0.txt or copy at [http://www.boost.org/LICENSE_1_0.txt](http://www.boost.org/LICENSE_1_0.txt))
