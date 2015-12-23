#pragma once
#include <dctl/action/ostream.hpp>
#include <dctl/state/state.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/actions.hpp>           // generate
#include <range/v3/all.hpp>             // is_permutation, transform, back_inserter
#include <boost/algorithm/string.hpp>   // trim_copy
#include <boost/test/unit_test.hpp>     // BOOST_CHECK, BOOST_CHECK_EQUAL
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
                std::vector<Action<Rules, Board>> moves;
                core::Actions<>{}.generate(p, moves);

                auto const N = ranges::size(rng);
                BOOST_CHECK_EQUAL(moves.size(), N);

                std::vector<std::string> notations;
                ranges::transform(moves, ranges::back_inserter(notations), [](auto const& m) {
                        return move::str_numeric(m);
                });

                using boost::algorithm::trim_copy;
                BOOST_CHECK(
                        ranges::is_permutation(
                                rng, notations, [](auto const& lhs, auto const& rhs) {
                                return trim_copy(lhs) == trim_copy(rhs);
                        })
                );
        }
};

}       // namespace core
}       // namespace dctl
