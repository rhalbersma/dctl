#pragma once
#include <dctl/action/ostream.hpp>
#include <dctl/state/state.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/actions.hpp>                             // generate
#include <boost/algorithm/cxx11/is_permutation.hpp>     // is_permutation
#include <boost/algorithm/string.hpp>                   // trim_copy
#include <boost/range/adaptor/transformed.hpp>          // transformed
#include <boost/test/unit_test.hpp>                     // BOOST_CHECK, BOOST_CHECK_EQUAL
#include <functional>                                   // cref
#include <string>                                       // string
#include <vector>                                       // vector

namespace dctl {
namespace core {

template<class Rules, class Board>
struct Fixture
{
        template<class Range>
        void test(std::string const& fen, Range const& rng)
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>{}(fen);
                std::vector<action<Rules, Board>> moves;
                core::Actions<>{}.generate(p, moves);

                BOOST_CHECK_EQUAL(moves.size(), rng.size());

                auto const move_str = [](auto const& m) { return move::str_numeric(m); };
                auto const notations = moves | boost::adaptors::transformed(std::cref(move_str));

                BOOST_CHECK(
                        boost::algorithm::is_permutation(
                                rng, notations.begin(), [](auto const& lhs, auto const& rhs) {
                                return
                                        boost::algorithm::trim_copy(lhs) ==
                                        boost::algorithm::trim_copy(rhs)
                                ;
                        })
                );
        }
};

}       // namespace core
}       // namespace dctl
