#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // dir_...
#include <boost/hana/tuple.hpp>                 // tuple_c

namespace dctl::core {
namespace block_adl {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct international
{
        // 2. The material
        constexpr static auto width                     = 10;           // 2.1
        constexpr static auto height                    = 10;           // 2.1
        constexpr static auto is_inverted               = false;        // 2.4
        constexpr static auto initial_position_gap      = 2;            // 2.8

        // 3. The movement of the piece
        constexpr static auto is_long_ranged_king       = true;         // 3.9

        // 4. The capture
        constexpr static auto is_backward_pawn_jump     = true;         // 4.1
        constexpr static auto is_superior_rank_jump     = false;        // 4.2 (implied by 3.1)
        constexpr static auto is_passing_capture        = false;        // 4.11
        constexpr static auto is_quantity_precedence    = true;         // 4.13
        constexpr static auto is_contents_precedence    = false;        // 4.13
        constexpr static auto is_modality_precedence    = false;        // 4.14

        constexpr static auto pawn_jump_directions = boost::hana::tuple_c<int, dir_NE, dir_NW, dir_SW, dir_SE>;

        struct msvc_workaround
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.num_captured_pieces();
                }
        };

        constexpr static auto precedence = msvc_workaround{};

        constexpr static auto is_passing_promotion      = false;        // 4.15 (and 3.5)

        // 6. The draw
        constexpr static auto max_repetitions           = 3;            // 6.1
        constexpr static auto max_reversible_moves      = 50;           // 6.2

        // 8. Notation
        constexpr static auto is_algebraic_notation     = false;        // 8.1 (and 2.6)
        constexpr static auto pushsep                   = '-';          // 8.2.2
        constexpr static auto jumpsep                   = 'x';          // 8.2.3
};

}       // namespace block_adl

using block_adl::international;

using brazilian  = international;
using philippine = international;
using canadian   = international;
using srilankan  = international;

}       // namespace dctl::core
