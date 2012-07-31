#pragma once
#include <algorithm>                    // transform
#include <cstddef>                      // size_t
#include <iterator>                     // back_inserter
#include <string>                       // string
#include <vector>                       // vector
#include <boost/test/unit_test.hpp>
#include "../../../src/successor/Generate.hpp"
#include "../../../src/node/Position.hpp"
#include "../../../src/node/Stack.hpp"
#include "../../../src/setup/Setup.hpp"
#include "../../../src/notation/String.hpp"
#include "../../../src/utility/is_permutation.hpp"

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
                Stack moves;
                successor::generate(p, moves);

                // check whether the number of legal moves is the same as legal's array length
                BOOST_CHECK(moves.size() == N);

                // write each move as a string into the vector notations
                std::vector<std::string> notations;
                std::transform(
                        std::begin(moves), std::end(moves), 
                        std::back_inserter(notations), 
                        [&](Move const& m){
                                return notation::write(p, m);
                        }
                );

                // check whether the list of generated moves is a permutation of the legal array
                // (moves could be generated in a different order by different engines)
                BOOST_CHECK(boost::algorithm::is_permutation(std::begin(legal), std::end(legal), std::begin(notations)));
        }
};

}       // namespace successor
}       // namespace dctl
