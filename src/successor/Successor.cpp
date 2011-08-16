#include <algorithm>    // std::find
#include <cstddef>      // std::size_t
#include <string>       // std::string
#include "gtest/gtest.h"
#include "../../../DCTL/src/successor/Successor.h"
#include "../../../DCTL/src/node/Position.h"
#include "../../../DCTL/src/node/Selection.h"
#include "../../../DCTL/src/setup/Setup.h"
#include "../../../DCTL/src/notation/String.h"
#include "../../../DCTL/src/board/Types.h"
#include "../../../DCTL/src/variant/Variant.h"

namespace dctl {

TEST(Successor, Italian)
{
        // Test positions from the official Italian rules: 
        // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
        std::string position[] = {
                "W:W30:B27",                    // Art. 5.6
                "W:W31:B12,20,28",              // Art. 5.7
                "W:WK22:B18,19,27",             // Art. 5.8
                "W:WK27:B5,6,13,15,21,23",      // Art. 5.9
                "W:WK21,31:B10,18,27",          // Art. 6.6
                "W:WK23,31:B19,26,27",          // Art. 6.7
                "W:WK23:B19,K28",               // Art. 6.8
                "W:WK30:B10,12,18,K20,K26,27",  // Art. 6.9
                "W:WK31:B18,20,K27,K28"         // Art. 6.10
        };

        int size[] = { 1, 1, 3, 1, 1, 1, 1, 1, 2 };

        std::string legal_56[] = { "30x23" };
        std::string legal_57[] = { "31x8 " };
        std::string legal_58[] = { "22x13", "22x15", "22x31" };
        std::string legal_59[] = { "27x25" };
        std::string legal_66[] = { "31x6 " };
        std::string legal_67[] = { "23x21" };
        std::string legal_68[] = { "23x32" };
        std::string legal_69[] = { "30x5 " };
        std::string legal_610[] = { "31x13", "31x15" };

        std::string* legal[] = {
                legal_56, legal_57, legal_58, legal_59,
                legal_66, legal_67, legal_68, legal_69, legal_610
        };

        for (auto i = 0; i < 9; ++i) {
                auto p = setup::read<board::Roman, pdn::protocol>()(position[i]);
                Stack moves;
                Successor<move::Legal, variant::Italian>::generate(p, moves);

                // check the number of generated legal
                EXPECT_EQ(size[i], moves.size());

                // check all generated legal
                for (auto j = 0; j < static_cast<int>(moves.size()); ++j) {
                        std::string move_string = notation::write<variant::Italian>()(p, moves[j]);
                        EXPECT_NE(legal[i] + size[i], std::find(legal[i], legal[i] + size[i], move_string)); 
                }
        }
}

}       // namespace dctl
