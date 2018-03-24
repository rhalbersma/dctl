#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>                      // uint8_t

namespace dctl::algo {
namespace search {

using PlyCount = uint8_t;

enum : PlyCount { AVG_MOVES = 16 };
enum : PlyCount { MAX_MOVES = 128 };
enum : PlyCount { MAX_PLY = 255 };
enum : PlyCount { MAX_MATE_MOVES = 255 };

}       // namespace search
}       // namespace dctl::algo
