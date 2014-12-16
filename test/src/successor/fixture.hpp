#pragma once
#include <dctl/move/ostream.hpp>
#include <dctl/position/position.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/successor.hpp>           // generate
#include <boost/algorithm/string.hpp>   // trim_copy
#include <boost/test/unit_test.hpp>     // BOOST_CHECK, BOOST_CHECK_EQUAL
#include <algorithm>                    // is_permutation, transform
#include <iterator>                     // back_inserter, begin, distance
#include <string>                       // string
#include <vector>                       // vector

namespace dctl {
namespace successor {

template<class Rules, class Board>
struct Fixture
{
        template<class Iterator>
        void run(std::string const& FEN, Iterator first, Iterator last)
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>{}(FEN);
                auto const moves = successor::generate(p);

                auto const N = std::distance(first, last);
                BOOST_CHECK_EQUAL(moves.size(), N);

                std::vector<std::string> notations;
                std::transform(begin(moves), end(moves), std::back_inserter(notations), [](auto const& m) {
                        return move::str_numeric(m);
                });

                using boost::algorithm::trim_copy;
                BOOST_CHECK(
                        std::is_permutation(
                                first, last, begin(notations), [](auto const& lhs, auto const& rhs) {
                                return trim_copy(lhs) == trim_copy(rhs);
                        })
                );
        }
};

}       // namespace successor
}       // namespace dctl
