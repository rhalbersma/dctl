#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

// http://www.damweb.cz/pravidla/cdfull.html
struct czech
{
        constexpr static auto width                     = 8;            // 2.1
        constexpr static auto height                    = 8;            // 2.1
        constexpr static auto coloring                  = 1;            // 2.4
        constexpr static auto is_algebraic_notation     = true;         // 2.6
        constexpr static auto jumpsep                   = ':';

        constexpr static auto is_long_ranged_king       = true;         // 3.9
        constexpr static auto is_backward_pawn_jump     = false;        // 4.1
        constexpr static auto is_modality_precedence    = true;         // 4.14

        constexpr static auto precedence = [](auto const& a) {
                return a.is_with_king();
        };
};

}       // namespace block_adl

using block_adl::czech;

}       // namespace dctl::core
