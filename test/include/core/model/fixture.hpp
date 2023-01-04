#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/action.hpp>
#include <dctl/core/board.hpp>
#include <dctl/core/model.hpp>                  // drop_duplicates_gen
#include <dctl/core/state.hpp>
#include <boost/algorithm/string.hpp>           // trim_copy
#include <boost/test/unit_test.hpp>             // BOOST_CHECK, BOOST_CHECK_EQUAL
#include <algorithm>                            // is_permutation, transform
#include <functional>                           // cref
#include <string>                               // string
#include <vector>                               // vector

namespace dctl::core {

template<class Rules, class Board = basic_board<Rules>>
struct Fixture
{
        template<class Range>
        void test(std::string const& fen, Range const& rng)
        {
                auto const state = pdn::read<Rules, Board>{}(fen);
                std::vector<basic_action<Rules, Board>> actions;
                drop_duplicates_gen.generate(state, actions);

                BOOST_CHECK_EQUAL(actions.size(), rng.size());

                auto const action_str = [](auto const& action) { return str_numeric(action); };
                std::vector<std::string> notations;
                std::ranges::transform(actions, std::back_inserter(notations), std::cref(action_str));

                BOOST_CHECK(
                        std::ranges::is_permutation(
                                rng, notations, [](auto const& lhs, auto const& rhs) {
                                return
                                        boost::algorithm::trim_copy(lhs) ==
                                        boost::algorithm::trim_copy(rhs)
                                ;
                        })
                );
        }
};

}       // namespace dctl::core
