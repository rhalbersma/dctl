#pragma once
#include <algorithm>                            // is_permutation, transform
#include <iterator>                             // back_inserter, begin, end
#include <string>                               // string
#include <vector>                               // vector
#include <boost/algorithm/string.hpp>           // trim_copy
#include <boost/test/unit_test.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/node/position.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/move/move.hpp>

namespace dctl {
namespace successor {

template<class Rules, class Board>
struct Fixture
{
        template<int N>
        void run(std::string const& FEN, std::string const (&legal)[N])
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>{}(FEN);
                auto const moves = successor::generate(p);

                BOOST_CHECK_EQUAL(static_cast<int>(moves.size()), N);

                std::vector<std::string> notations;
                notations.reserve(moves.size());
                std::transform(
                        begin(moves), end(moves),
                        std::back_inserter(notations),
                        [](auto const& m) {
                        return m.numeric();
                });

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
