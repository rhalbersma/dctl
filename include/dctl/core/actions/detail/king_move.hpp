#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/tables.hpp>  // king_moves
#include <dctl/core/board/bearing.hpp>          // bearing
#include <dctl/core/rules/type_traits.hpp>      // is_long_ranged_king_t
#include <dctl/core/state/color_piece.hpp>      // color, color_, king_
#include <dctl/util/meta.hpp>                   // foldl_logical_or, foldl_plus, foldl_comma
#include <dctl/util/type_traits.hpp>            // board_t, rules_t

namespace dctl::core {
namespace detail {

template<class...>
class king_move;

template<color Side, class Reverse, class State>
class king_move<color_<Side>, Reverse, State>
{
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;
public:
        static auto detect(State const& s) noexcept
        {
                return s.pieces(color_c<Side>, kings_c).any_of([occup = s.pieces(occup_c)](auto from_sq) {
                        return !king_moves<rules_type, board_type>{}(from_sq, occup).empty();
                });
        }

        static auto count(State const& s) noexcept
        {
                return s.pieces(color_c<Side>, kings_c).accumulate(0, [occup = s.pieces(occup_c)](auto result, auto from_sq) {
                        return result + king_moves<rules_type, board_type>{}(from_sq, occup).count();
                });
        }

        template<class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                s.pieces(color_c<Side>, kings_c).consume([&seq, occup = s.pieces(occup_c)](auto from_sq) {
                        king_moves<rules_type, board_type>{}(from_sq, occup).consume([&](auto dest_sq) {
                                seq.emplace_back(from_sq, dest_sq);
                        });
                });
        }
};

}       // namespace detail
}       // namespace dctl::core
