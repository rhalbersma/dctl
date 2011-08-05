#include <algorithm>    // std::find
#include <string>
#include "gtest/gtest.h"
#include "../../../DCTL/src/generate/Successors.h"
#include "../../../DCTL/src/node/Position.h"
#include "../../../DCTL/src/board/Types.h"
#include "../../../DCTL/src/rules/Rules.h"

#include "../../../DCTL/src/setup/Diagram.h"
#include "../../../DCTL/src/setup/String.h"
#include "../../../DCTL/src/notation/String.h"

namespace dctl {
namespace generate {

/*
// Test positions from the official Italian rules: http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
TEST(MoveGeneration, ItalianBoard)
{
        Position<board::Roman> ITA_empty = string::read<board::Roman, pdn::protocol>()("");   // Art. 2.1
        typedef board::Roman ITA_notation;                                      	                        // Art. 2.4
        Position<board::Roman> ITA_initial;						                // Art. 2.6

        std::cout << layout::write<pdn::protocol>()(ITA_empty) << std::endl;
        board::Diagram<ITA_notation, board::squares>()();
        std::cout << layout::write<pdn::protocol>()(ITA_initial) << std::endl;
}
*/
TEST(Successors, Italian)
{
        // Test positions from the official Italian rules: http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
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

        size_t size[] = { 1, 1, 3, 1, 1, 1, 1, 1, 2 };

        std::string moves_56[] = { "30x23" };
        std::string moves_57[] = { "31x8 " };
        std::string moves_58[] = { "22x13", "22x15", "22x31" };
        std::string moves_59[] = { "27x25" };
        std::string moves_66[] = { "31x6 " };
        std::string moves_67[] = { "23x21" };
        std::string moves_68[] = { "23x32" };
        std::string moves_69[] = { "30x5 " };
        std::string moves_610[] = { "31x13", "31x15" };

        std::string* moves[] = {
                moves_56, moves_57, moves_58, moves_59,
                moves_66, moves_67, moves_68, moves_69, moves_610
        };

        for (auto i = 0; i < 9; ++i) {
                Stack move_stack;
                auto p = setup::read<board::Roman, pdn::protocol>()(position[i]);
                Successors<rules::Italian, board::Roman>::generate(p, move_stack);

                // check the number of generated moves
                EXPECT_EQ(size[i], move_stack.size());

                // check all generated moves
                for (size_t j = 0; j < move_stack.size(); ++j) {
                        std::string move_string = notation::write<rules::Italian>()(p, move_stack[j]);
                        EXPECT_NE(moves[i] + size[i], std::find(moves[i], moves[i] + size[i], move_string)); 
                }
        }
}

}       // namespace generate
}       // namespace dctl
