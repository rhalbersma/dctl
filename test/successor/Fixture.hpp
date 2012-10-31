#pragma once
#include <algorithm>                    // transform
#include <cstddef>                      // size_t
#include <iterator>                     // back_inserter, begin, end
#include <string>                       // string
#include <vector>                       // vector
#include <boost/algorithm/string.hpp>   // trim_copy
#include <boost/test/unit_test.hpp>
#include <dctl/successor/Generate.hpp>
#include <dctl/node/Position.hpp>
#include <dctl/node/Stack.hpp>
#include <dctl/setup/Setup.hpp>
#include <dctl/notation/String.hpp>
#include <dctl/guarded/is_permutation.hpp>

namespace dctl {
namespace successor {

template<typename Rules, typename Board>
struct Fixture
{
        template<std::size_t N>
        void run(std::string const& FEN, std::string const (&legal)[N])
        {
                // setup the position and generate all legal moves
                auto const p = setup::read<Rules, Board, pdn::protocol>()(FEN);
                auto const moves = successor::generate(p);

                // check whether the number of generated moves is equal to the number of legal moves
                BOOST_CHECK(moves.size() == N);

                // write each move as a string into the vector notations
                std::vector<std::string> notations;
                std::transform(
                        std::begin(moves), std::end(moves),
                        std::back_inserter(notations),
                        [&](Move const& m) {
                        return (notation::write(p, m));
                });

                // check whether the vector of generated moves is a permutation of the array of legal moves
                BOOST_CHECK(
                        boost::algorithm::is_permutation(
                                std::begin(legal), std::end(legal),
                                std::begin(notations), 
                                [](std::string const& lhs, std::string const& rhs) {
                                return (boost::algorithm::trim_copy(lhs) == boost::algorithm::trim_copy(rhs));
                        })
                );
        }
};

}       // namespace successor
}       // namespace dctl
