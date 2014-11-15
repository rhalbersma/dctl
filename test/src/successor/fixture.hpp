#pragma once
#include <algorithm>                                    // is_permutation, transform
#include <iterator>                                     // back_inserter, begin, end
#include <string>                                       // string
#include <vector>                                       // vector
#include <boost/algorithm/string.hpp>                   // trim_copy
#include <boost/range/adaptor/transformed.hpp>          // transformed
#include <boost/range/algorithm_ext/push_back.hpp>      // push_back
#include <boost/test/unit_test.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/position/position.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/move/move.hpp>
#include <dctl/move/ostream.hpp>

namespace dctl {
namespace successor {

template<class Rules, class Board>
struct Fixture
{
        template<std::size_t N>
        void run(std::string const& FEN, std::string const (&legal)[N])
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>{}(FEN);
                auto const moves = successor::generate(p);

                BOOST_CHECK_EQUAL(moves.size(), N);

                std::vector<std::string> notations;
                boost::push_back(notations, moves | boost::adaptors::transformed([](auto const& m) {
                        return move::str_numeric(m);
                }));

                using boost::algorithm::trim_copy;
                BOOST_CHECK(
                        std::is_permutation(
                                std::begin(legal), std::end(legal), begin(notations), [](auto const& lhs, auto const& rhs) {
                                return trim_copy(lhs) == trim_copy(rhs);
                        })
                );
        }
};

}       // namespace successor
}       // namespace dctl
