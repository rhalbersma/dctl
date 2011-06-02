#include <algorithm>
#include <string>
#include "gtest/gtest.h"
#include "../../../../Library/Source/Tree/Generate/Successors.h"
#include "../../../../Library/Source/Tree/Node/Position.h"
#include "../../../../Library/Source/Tree/Node/Protocol.h"
#include "../../../../Library/Source/Tree/Node/String.h"
#include "../../../../Library/Source/Tree/Move/String.h"
#include "../../../../Library/Source/Geometry/Board.h"
#include "../../../../Library/Source/Geometry/Layout.h"
#include "../../../../Library/Source/Variants/Rules.h"

namespace Tree {
namespace Generate {

/*
// Test positions from the official Italian rules: http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
TEST(MoveGeneration, ItalianBoard)
{
        Node::Position<Geometry::Roman> ITA_empty = Node::String::read<Geometry::Roman, Node::FEN_tag>()("");   // Art. 2.1
        typedef Geometry::Roman ITA_notation;                                      	                        // Art. 2.4
        Node::Position<Geometry::Roman> ITA_initial;						                // Art. 2.6

        std::cout << Node::Layout::write<Node::FEN_tag>()(ITA_empty) << std::endl;
        Geometry::Layout::write<ITA_notation, Geometry::Layout::Square_tag>()();
        std::cout << Node::Layout::write<Node::FEN_tag>()(ITA_initial) << std::endl;
}
*/
TEST(MoveGeneration, Italian)
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

        for (size_t i = 0; i < 9; ++i) {
                Move::Stack move_stack;
                Node::Position<Geometry::Roman> p = Node::String::read<Geometry::Roman, Node::FEN_tag>()(position[i]);
                Successors<Variants::Italian, Geometry::Roman>::generate(p, move_stack);

                // check the number of generated moves
                EXPECT_EQ(size[i], move_stack.size());

                // check all generated moves
                for (size_t j = 0; j < move_stack.size(); ++j) {
                        std::string move_string = Move::String::write<Variants::Italian>()(p, move_stack[j]);
                        EXPECT_NE(moves[i] + size[i], std::find(moves[i], moves[i] + size[i], move_string)); 
                }
        }
}

}       // namespace Generate
}       // namespace Tree
