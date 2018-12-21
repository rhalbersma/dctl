#pragma once

//          Copyright Rein Halbersma 2010-2018.
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
        constexpr static auto width = 8;                        // 2.1.2
        constexpr static auto height = 8;                       // 2.1.2
        constexpr static auto is_inverted = true;               // 2.1.5

        constexpr static auto is_long_ranged_king = true;
        constexpr static auto is_backward_pawn_jump = false;

        constexpr static auto is_quantity_precedence = true;
        constexpr static auto is_contents_precedence = true;

        struct msvc_workaround
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured_pieces(),
                                a.num_captured_kings()
                        );
                }
        };

        constexpr static auto precedence = msvc_workaround{};
};

}       // namespace block_adl

using block_adl::spanish;

}       // namespace dctl::core
