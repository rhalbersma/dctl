#pragma once
#include <cstddef>                      // size_t
#include <string>                       // string
#include <boost/test/unit_test.hpp>
//#include <boost/algorithm/cxx11/is_permutation.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "../../../src/utility/Algorithm.hpp"
#include "../../../src/successor/Selection.hpp"
#include "../../../src/successor/Successor.hpp"
#include "../../../src/node/Position.hpp"
#include "../../../src/setup/Setup.hpp"
#include "../../../src/notation/String.hpp"

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
        void run(std::string const& FEN, std::string (&legal)[N])
        {
                auto const p = setup::read<Rules, Board, pdn::protocol>()(FEN);
                Stack moves;
                Successor<select::Legal>::generate(p, moves);

                BOOST_CHECK(moves.size() == N);

                std::vector<std::string> notations(moves.size());
                std::transform(
                        std::begin(moves), std::end(moves), std::begin(notations), [&p](Move const& m){
                        return notation::write<Rules>()(p, m);
                });

                BOOST_CHECK(is_permutation(std::begin(legal), std::end(legal), std::begin(notations)));
        }
};

}       // namespace successor
}       // namespace dctl
