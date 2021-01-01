#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/util/type_traits.hpp> // action_t
#include <cassert>                      // assert

namespace dctl::algo {

template<class State, class Actions, class UnaryPredicate>
class problem
{
        State const initial_;
        Actions const actions_;
        std::optional<UnaryPredicate> const goal_test_;

        using  state_type = State;
        using action_type = action_t<Actions>;
public:
        problem(State const& root, Actions succ, UnaryPredicate pred) noexcept
        :
                initial_{root},
                actions_{succ},
                goal_test_{pred}
        {}

        auto initial() const noexcept
        {
                return initial_;
        }

        template<class State>
        auto actions(State const& s) const
        {
                return actions_(s);
        }

        template<class State, class Action>
        auto result(State const& s, Action const& a) const noexcept
        {
                auto nrv{s}; nrv.make(a); return nrv;
        }

        template<class State>
        auto goal_test(State const& s) const
        {
                assert(goal_test_);
                return *goal_test_(s);
        }
};

}       // namespace dctl::algo
