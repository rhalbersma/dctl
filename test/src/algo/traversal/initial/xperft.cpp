//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/traversal/root.hpp> // perft_state
#include <dctl/core.hpp>
#include <cstddef>
#include <vector>

using namespace dctl::algo;
using namespace dctl::core;

int main()
{
        using state = basic_state<international, basic_board<international>>;

        auto const states = std::vector<state>{
                state::initial(),
                pdn::read<international>{}
                        ("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"),
                pdn::read<international>{}
                        ("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"),
                pdn::read<international>{}
                        ("W:WK31,K32,K33,K34,K35,K36,K37,K38,K39,K40,K41,K42,K43,K44,K45,K46,K47,K48,K49,K50:BK1,K2,K3,K4,K5,K6,K7,K8,K9,K10,K11,K12,K13,K14,K15,K16,K17,K18,K19,K20"),
                pdn::read<international>{}
                        ("W:W6,7,8,9,10:B41,42,43,44,45")
        };

        auto const depths = std::vector<int>{ 11, 9, 15, 11, 9 };

        for (auto i = std::size_t(0); i < states.size(); ++i) {
                traversal::perft(states[i], depths[i], drop_duplicates_gen);
        }
}
