#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/detail/king_moves.hpp>        // king_moves
#include <dctl/core/model/detail/pawn_moves.hpp>        // pawn_moves
#include <dctl/core/state/color.hpp>                    // color, color_
#include <dctl/util/type_traits.hpp>                    // board_t, rules_t

namespace dctl::core::detail {

template<class...>
class moves;

template<color Side, class ReverseGenerator>
class moves<color_<Side>, ReverseGenerator>
{
        template<class State> using pawn_moves = detail::pawn_moves<rules_t<State>, board_t<State>, color_<Side>, ReverseGenerator>;
        template<class State> using king_moves = detail::king_moves<rules_t<State>, board_t<State>                                >;
public:
        template<class State>
        [[nodiscard]] static constexpr auto detect(State const& s) noexcept
        {
                return
                        pawn_moves<State>::detect(s.pieces(color_c<Side>, pawn_c), s.pieces(empty_c)) ||
                        king_moves<State>::detect(s.pieces(color_c<Side>, king_c), s.pieces(empty_c))
                ;
        }

        template<class State>
        [[nodiscard]] static constexpr auto count(State const& s) noexcept
        {
                return
                        pawn_moves<State>::count(s.pieces(color_c<Side>, pawn_c), s.pieces(empty_c)) +
                        king_moves<State>::count(s.pieces(color_c<Side>, king_c), s.pieces(empty_c))
                ;
        }

        template<class State>
        static constexpr auto generate(State const& s, auto& actions) noexcept
        {
                pawn_moves<State>::generate(s.pieces(color_c<Side>, pawn_c), s.pieces(empty_c), actions);
                king_moves<State>::generate(s.pieces(color_c<Side>, king_c), s.pieces(empty_c), actions);
        }
};

}       // namespace dctl::core::detail
