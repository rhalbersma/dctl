#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>        // make_tuple

namespace dctl::core {
namespace block_adl {

// http://www.fpdamas.pt/downloads/Regras%20Damas%20Cl%C3%A1ssicas%20-%20atualizado.pdf
struct spanish
{
        // 2.1 NOMENCLATURA DO TABULEIRO
        static constexpr auto width                     = 8;    // 2.1.2
        static constexpr auto height                    = 8;    // 2.1.2
        static constexpr auto coloring                  = 0;    // 2.1.5

        static constexpr auto is_long_ranged_king       = true;
        static constexpr auto is_backward_pawn_jump     = false;

        static constexpr auto is_quantity_precedence    = true;
        static constexpr auto is_contents_precedence    = true;

        static constexpr auto precedence = [](auto const& a) {
                return std::make_tuple(
                        a.num_captured_pieces(),
                        a.num_captured_kings()
                );
        };
};

}       // namespace block_adl

using block_adl::spanish;

}       // namespace dctl::core
