#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board.hpp>
#include <dctl/core/action.hpp>
#include <dctl/core/actions.hpp>                // generate
#include <dctl/core/state.hpp>
#include <boost/algorithm/string.hpp>           // trim_copy
#include <boost/test/unit_test.hpp>             // BOOST_CHECK, BOOST_CHECK_EQUAL
#include <algorithm>                            // is_permutation, transform
#include <functional>                           // cref
#include <string>                               // string
#include <vector>                               // vector

namespace dctl::core {

template<class Rules, class Board = board<Rules>>
struct Fixture
{
        template<class Range>
        void test(std::string const& fen, Range const& rng)
        {
                auto const s = setup::read<Rules, Board>{}(fen);
                std::vector<basic_action<Rules, Board>> moves;
                actions<>{}.generate(s, moves);

                BOOST_CHECK_EQUAL(moves.size(), rng.size());

                auto const move_str = [](auto const& m) { return str_numeric(m); };
                auto notations = std::vector<std::string>{};
                std::transform(moves.begin(), moves.end(), std::back_inserter(notations), std::cref(move_str));

                BOOST_CHECK(
                        std::is_permutation(
                                rng.begin(), rng.end(), notations.begin(), [](auto const& lhs, auto const& rhs) {
                                return
                                        boost::algorithm::trim_copy(lhs) ==
                                        boost::algorithm::trim_copy(rhs)
                                ;
                        })
                );
        }
};

}       // namespace dctl::core
