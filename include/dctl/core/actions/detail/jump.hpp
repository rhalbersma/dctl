#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/action/action.hpp>                       // Action
#include <dctl/core/actions/detail/builder.hpp>              // builder
#include <dctl/core/actions/detail/detect_primary_fwd.hpp>   // detect (primary template)
#include <dctl/core/actions/detail/detect_king_jump.hpp>     // detect (king jump specialization)
#include <dctl/core/actions/detail/detect_pawn_jump.hpp>     // detect (pawn jump specialization)
#include <dctl/core/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/core/actions/detail/generate_king_jump.hpp>   // generate (king jump specialization)
#include <dctl/core/actions/detail/generate_pawn_jump.hpp>   // generate (pawn jump specialization)
#include <dctl/core/actions/select/jump.hpp>                    // jump
#include <dctl/core/state/color_piece.hpp>                      // color, color_, kings_, pawn_
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/type_traits.hpp>                            // rules_t, board_t
#include <boost/container/static_vector.hpp>
#include <cassert>                                              // assert
#include <type_traits>

namespace dctl::core {
namespace detail {

class MoveCounter
{
        int n{};
public:
        auto& operator++()
        {
                ++n; return *this;
        }

        auto size() const
        {
                return n;
        }
};

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::jump, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
public:
        template<class State, class SequenceContainer = boost::container::static_vector<action<rules_t<State>, board_t<State>>, 128>>
        auto generate(State const& s) const
        {
                SequenceContainer seq;
                generate(s, seq);
                return seq;
        }

        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& seq) const
        {
                using builder_type = builder<to_move_, DuplicatesPolicy, State, SequenceContainer>;

                //                /*std::conditional_t<
                //        std::is_same_v<Sequence, MoveCounter> && not DuplicatesPolicy,
                //        Counter<to_move_, DuplicatesPolicy, State>,*/
                //        ;
                //>;

                using king_jump = detail::generate<to_move_, kings_, select::jump, Reverse, State, builder_type>;
                using pawn_jump = detail::generate<to_move_, pawns_, select::jump, Reverse, State, builder_type>;

                auto b = builder_type{s, seq};
                king_jump{b}();
                pawn_jump{b}();
        }

        template<class State>
        auto count(State const& s) const
        {
                return static_cast<int>(generate(s).size());
        }

        template<class State>
        auto detect(State const& s) const noexcept
        {
                using pawn_jump = detail::detect<to_move_, pawns_, select::jump, Reverse, State>;
                using king_jump = detail::detect<to_move_, kings_, select::jump, Reverse, State>;

                return pawn_jump{}(s) || king_jump{}(s);
        }
};

}       // namespace detail
}       // namespace dctl::core
