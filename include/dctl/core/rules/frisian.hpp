#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // dir_...
#include <dctl/util/type_traits.hpp>    // board_t, set_t
#include <tabula/tuple.hpp>         // tuple_c
#include <tuple>                        // make_tuple
#include <type_traits>                  // decay_t

namespace dctl::core {
namespace block_adl {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct frisian
{
        constexpr static auto width                     = 10;
        constexpr static auto height                    = 10;
        constexpr static auto coloring                  = 1;

        constexpr static auto is_backward_pawn_jump     = true;
        constexpr static auto is_long_ranged_king       = true;

        constexpr static auto is_orthogonal_jumps       = true;
        constexpr static auto max_same_king_move        = 3;

        constexpr static auto is_quantity_precedence    = true;
        constexpr static auto is_contents_precedence    = true;
        constexpr static auto is_modality_precedence    = true;

        constexpr static auto pawn_jump_directions = tabula::tuple_c<int, dir_E, dir_NE, dir_N, dir_NW, dir_W, dir_SW, dir_S, dir_SE>;
        constexpr static auto king_jump_directions = tabula::tuple_c<int, dir_E, dir_NE, dir_N, dir_NW, dir_W, dir_SW, dir_S, dir_SE>;

        constexpr static auto precedence = [](auto const& a) {
                constexpr auto max_captured_pieces = board_t<std::decay_t<decltype(a)>>::bits();
                auto const num_captured_kings = a.num_captured_kings();
                return std::make_tuple(
                        (a.num_captured_pieces() + num_captured_kings) * max_captured_pieces - num_captured_kings,
                        a.is_with_king()
                );
        };
};

}       // namespace block_adl

using block_adl::frisian;

}       // namespace dctl::core
