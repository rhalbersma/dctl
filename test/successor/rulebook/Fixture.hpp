#pragma once
#include <cstddef>                      // size_t
#include <iterator>                     // back_inserter
#include <string>                       // string
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "../../../src/utility/algorithm.hpp"
#include "../../../src/successor/Generate.hpp"
#include "../../../src/successor/Select.hpp"
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
        Fixture()
        {
        }

        ~Fixture()
        {
        }

        template<std::size_t N>
        void run(std::string const& FEN, std::string const (&legal)[N])
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>()(FEN);
                Stack moves;
                successor::generate<select::Legal>(p, moves);

                BOOST_CHECK(moves.size() == N);

                std::vector<std::string> notations;
                std::transform(
                        std::begin(moves), std::end(moves), 
                        std::back_inserter(notations), 
                        [&](Move const& m){
                                return notation::write(p, m);
                        }
                );

                BOOST_CHECK(boost::algorithm::is_permutation(std::begin(legal), std::end(legal), std::begin(notations)));
        }
};

}       // namespace successor
}       // namespace dctl
