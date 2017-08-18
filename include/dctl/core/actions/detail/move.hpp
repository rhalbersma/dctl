#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/king_move.hpp>       // king_move
#include <dctl/core/actions/detail/pawn_move.hpp>       // pawn_move
#include <dctl/core/actions/detail/primary_fwd.hpp>     // actions (primary template)
#include <dctl/core/actions/select/move.hpp>            // move
#include <dctl/core/state/color_piece.hpp>              // color, color_
#include <dctl/util/type_traits.hpp>                    // board_t, rules_t

namespace dctl::core {
namespace detail {

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::move, DuplicatesPolicy, Reverse>
{
        template<class State> using pawn_move = detail::pawn_move<rules_t<State>, board_t<State>, color_<Side>, Reverse>;
        template<class State> using king_move = detail::king_move<rules_t<State>, board_t<State>>;
public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return
                        pawn_move<State>::detect(s.pieces(color_c<Side>, pawns_c), s.pieces(empty_c)) ||
                        king_move<State>::detect(s.pieces(color_c<Side>, kings_c), s.pieces(empty_c))
                ;
        }

        template<class State>
        static auto count(State const& s) noexcept
        {
                return
                        pawn_move<State>::count(s.pieces(color_c<Side>, pawns_c), s.pieces(empty_c)) +
                        king_move<State>::count(s.pieces(color_c<Side>, kings_c), s.pieces(empty_c))
                ;
        }

        template<class State, class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                pawn_move<State>::generate(s.pieces(color_c<Side>, pawns_c), s.pieces(empty_c), seq);
                king_move<State>::generate(s.pieces(color_c<Side>, kings_c), s.pieces(empty_c), seq);
        }
};

}       // namespace detail
}       // namespace dctl::core
