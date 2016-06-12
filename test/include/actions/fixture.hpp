#pragma once
#include <dctl/action/ostream.hpp>
#include <dctl/state/state.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/actions.hpp>             // generate
#include <boost/algorithm/string.hpp>   // trim_copy
#include <boost/test/unit_test.hpp>     // BOOST_CHECK, BOOST_CHECK_EQUAL
#include <algorithm>                    // is_permutation, transform
#include <iterator>                     // back_inserter
#include <string>                       // string
#include <vector>                       // vector

namespace dctl {
namespace core {

template<class Rules, class Board>
struct Fixture
{
        template<class Range>
        void test(std::string const& FEN, Range const& rng)
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>{}(FEN);
                std::vector<action<Rules, Board>> moves;
                core::Actions<>{}.generate(p, moves);

                auto const N = rng.size();
                BOOST_CHECK_EQUAL(moves.size(), N);

                std::vector<std::string> notations;
                std::transform(moves.cbegin(), moves.cend(), std::back_inserter(notations), [](auto const& m) {
                        return move::str_numeric(m);
                });

                using boost::algorithm::trim_copy;
                BOOST_CHECK(
                        std::is_permutation(
                                rng.cbegin(), rng.cend(), notations.cbegin(), notations.cend(), [](auto const& lhs, auto const& rhs) {
                                return trim_copy(lhs) == trim_copy(rhs);
                        })
                );
        }
};

}       // namespace core
}       // namespace dctl
