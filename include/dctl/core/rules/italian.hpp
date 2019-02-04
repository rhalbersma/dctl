#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>        // make_tuple

namespace dctl::core {
namespace block_adl {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct italian
{
        // Art. 2 - Il materiale
        constexpr static auto width                     = 8;            // 2.1
        constexpr static auto height                    = 8;            // 2.1
        constexpr static auto coloring                  = 0;            // 2.3

        // Art. 4 - Gli spostamenti semplici
        constexpr static auto is_long_ranged_king       = false;        // 4.7

        // Art. 5 - Le prese
        constexpr static auto is_backward_pawn_jump     = false;        // 5.3(a)
        constexpr static auto is_superior_rank_jump     = true;         // 5.3(b)

        // Art. 6 - Le regole di presa
        constexpr static auto is_quantity_precedence    = true;         // 6.6
        constexpr static auto is_modality_precedence    = true;         // 6.7
        constexpr static auto is_contents_precedence    = true;         // 6.8
        constexpr static auto is_ordering_precedence    = true;         // 6.9

        constexpr static auto precedence = [](auto const& a) {
                return std::make_tuple(
                        a.num_captured_pieces(),
                        a.is_with_king(),
                        a.num_captured_kings(),
                        a.piece_order()
                );
        };

        constexpr static auto max_repetitions           = 4;            // 9.3(b1)
        constexpr static auto max_reversible_moves      = 80;           // 10.4
};

}       // namespace block_adl

using block_adl::italian;

}       // namespace dctl::core
