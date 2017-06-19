#pragma once
#include <dctl/core/action.hpp>
#include <dctl/core/actions.hpp>                // generate
#include <dctl/core/state.hpp>
#include <boost/algorithm/string.hpp>           // trim_copy
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/test/unit_test.hpp>             // BOOST_CHECK, BOOST_CHECK_EQUAL
#include <algorithm>
#include <functional>                           // cref
#include <string>                               // string
#include <vector>                               // vector

namespace dctl::core {

template<class Rules, class Board>
struct Fixture
{
        template<class Range>
        void test(std::string const& fen, Range const& rng)
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>{}(fen);
                std::vector<action<Rules, Board>> moves;
                actions<>{}.generate(p, moves);

                BOOST_CHECK_EQUAL(moves.size(), rng.size());

                auto const move_str = [](auto const& m) { return str_numeric(m); };
                auto const notations = moves | boost::adaptors::transformed(std::cref(move_str));

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
