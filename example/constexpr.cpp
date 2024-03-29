//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl.hpp>
#include <version>  // __cpp_lib_constexpr_vector

using namespace dctl::algo;
using namespace dctl::core;

int main()
{
/*
#ifdef __cpp_lib_constexpr_vector
  static_assert(
    traversal::depth_limited_count<true>( // perft with bulk-counting
      basic_state<
        international,
        basic_board<international>
      >::initial(),                       // the starting position in International Draughts
      7,                                  // depth = 7
      drop_duplicates_gen                 // no duplicate captures
    ) == 1'049'442
  );
#endif
*/
}
