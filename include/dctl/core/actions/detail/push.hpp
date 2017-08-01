#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/count_primary_fwd.hpp>    // count (primary template)
#include <dctl/core/actions/detail/count_king_push.hpp>      // count (king push specialization)
#include <dctl/core/actions/detail/count_pawn_push.hpp>      // count (pawn push specialization)
#include <dctl/core/actions/detail/detect_primary_fwd.hpp>   // detect (primary template)
#include <dctl/core/actions/detail/detect_king_push.hpp>     // detect (king push specialization)
#include <dctl/core/actions/detail/detect_pawn_push.hpp>     // detect (pawn push specialization)
#include <dctl/core/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/core/actions/detail/generate_king_push.hpp>   // generate (king push specialization)
#include <dctl/core/actions/detail/generate_pawn_push.hpp>   // generate (pawn push specialization)
#include <dctl/core/actions/select/push.hpp>                 // push
#include <dctl/core/state/color_piece.hpp>                   // color, color_, kings_, pawn_

namespace dctl::core {
namespace detail {

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::push, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& seq) const
        {
                using king_push = detail::generate<to_move_, kings_, select::push, Reverse, State>;
                using pawn_push = detail::generate<to_move_, pawns_, select::push, Reverse, State>;

                king_push{}(s, seq);
                pawn_push{}(s, seq);
        }

        template<class State>
        auto count(State const& s) const noexcept
        {
                using king_push = detail::count<to_move_, kings_, select::push, Reverse, State>;
                using pawn_push = detail::count<to_move_, pawns_, select::push, Reverse, State>;

                return king_push{}(s) + pawn_push{}(s);
        }

        template<class State>
        auto detect(State const& s) const noexcept
        {
                using pawn_push = detail::detect<to_move_, pawns_, select::push, Reverse, State>;
                using king_push = detail::detect<to_move_, kings_, select::push, Reverse, State>;

                return pawn_push{}(s) || king_push{}(s);
        }
};

}       // namespace detail
}       // namespace dctl::core
