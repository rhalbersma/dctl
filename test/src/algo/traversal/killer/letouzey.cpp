//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algo/traversal/fixture.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>                      // int64_t
#include <vector>

using namespace dctl::algo;
using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(TraversalKillerLetouzey)

// email conversation with Fabien Letouzey

BOOST_AUTO_TEST_CASE(Random178)
{
        auto const state = pdn::read<killer<international>>{}(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"
        );
        auto const leafs = std::vector<int64_t>
        {
                            14,
                            55,
                         1'168,
                         5'165,
                        84'326,
                       573'965,
                     8'476'150,
                    78'944'517,
                 1'133'545'351,
                12'070'611'997
        };
        traversal::test(state, leafs, drop_duplicates_gen);
}

BOOST_AUTO_TEST_CASE(Woldouby)
{
        auto const state = pdn::read<killer<international>>{}(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        );
        auto const leafs = std::vector<int64_t>{
                             6,
                            12,
                            30,
                            73,
                           215,
                           590,
                         1'944,
                         6'269,
                        22'369,
                        88'043,
                       377'339,
                     1'908'829,
                     9'850'540,
                    58'116'971,
                   343'968'214,
                 2'251'657'424,
                14'783'093'851
        };
        traversal::test(state, leafs, drop_duplicates_gen);
}

BOOST_AUTO_TEST_SUITE_END()
