#include "../../../../Library/Source/Tree/Walk/Root.h"
#include "../../../../Library/Source/Tree/Node/Position.h"
#include "../../../../Library/Source/Tree/Node/Protocol.h"
#include "../../../../Library/Source/Tree/Node/String.h"
#include "../../../../Library/Source/Geometry/Board.h"
#include "../../../../Library/Source/Geometry/Layout.h"
#include "../../../../Library/Source/Variants/Rules.h"
#include "gtest/gtest.h"

namespace Tree {
namespace Walk {

#if ACCEPTANCE_TEST == 1

// The fixture for testing class SearchEndgame.
class Perft: public ::testing::Test {
protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Perft() {
        // You can do set-up work for each test here.
        }

        virtual ~Perft() {
        // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        }

        virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
}

// Objects declared here can be used by all tests in the test case for Foo.
};

// The original perft thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2308
TEST_F(Perft, International)
{
        std::cout << Geometry::Layout::write<Geometry::International, Geometry::Layout::Square_tag>()() << std::endl;
        std::cout << Geometry::Layout::write<Geometry::International, Geometry::Layout::Bit_tag>()() << std::endl;

        Node::Position<Geometry::International> i10 = Node::Position<Geometry::International>::initial();
        Root::perft<Variants::International>(i10, 11);

        Node::Position<Geometry::International> random178(Node::String::read<Geometry::International, Node::FEN_tag>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Root::perft<Variants::International>(random178, 9);

        Node::Position<Geometry::International> Woldouby(Node::String::read<Geometry::International, Node::FEN_tag>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        Root::perft<Variants::International>(Woldouby, 15);
}

// The rectangular board thread on the FMJD forum: http://laatste.info/bb3/viewtopic.php?f=53&t=3014
TEST_F(Perft, Rectangular)
{
        std::cout << Geometry::Layout::write<Geometry::Spantsireti, Geometry::Layout::Square_tag>()() << std::endl;
        std::cout << Geometry::Layout::write<Geometry::Spantsireti, Geometry::Layout::Bit_tag>()() << std::endl;

        Node::Position<Geometry::Spantsireti> iS = Node::Position<Geometry::Spantsireti>::initial();
        Root::perft<Variants::International>(iS, 9);

        std::cout << Geometry::Layout::write<Geometry::Ktar11, Geometry::Layout::Square_tag>()() << std::endl;
        std::cout << Geometry::Layout::write<Geometry::Ktar11, Geometry::Layout::Bit_tag>()() << std::endl;
        
        Node::Position<Geometry::Ktar11> iK11 = Node::Position<Geometry::Ktar11>::initial();
        Root::perft<Variants::International>(iK11, 9);
                
        std::cout << Geometry::Layout::write<Geometry::Ktar12, Geometry::Layout::Square_tag>()() << std::endl;
        std::cout << Geometry::Layout::write<Geometry::Ktar12, Geometry::Layout::Bit_tag>()() << std::endl;
        
        Node::Position<Geometry::Ktar12> iK12 = Node::Position<Geometry::Ktar12>::initial();
        Root::perft<Variants::International>(iK12, 9);
}

// The alternative game rules thread on the FMJD forum: http://laatste.info/bb3/viewtopic.php?f=53&t=2822
TEST_F(Perft, ChessVariants)
{
        Node::Position<Geometry::Chess> i8 = Node::Position<Geometry::Chess>::initial();
        std::cout << Geometry::Layout::write<Geometry::Chess, Geometry::Layout::Square_tag>()() << std::endl;
        
        Root::perft<Variants::Brazilian>(i8, 13); // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Root::perft<Variants::Pool>(i8, 13);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Root::perft<Variants::Russian>(i8, 14);   // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::perft<Variants::English>(i8, 14);   // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::perft<Variants::Czech>(i8, 13);

        Node::Position<Geometry::Roman> r8 = Node::Position<Geometry::Roman>::initial();
        std::cout << Geometry::Layout::write<Geometry::Roman, Geometry::Layout::Square_tag>()() << std::endl;

        Root::perft<Variants::Spanish>(r8, 13);
        Root::perft<Variants::Italian>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        Node::Position<Geometry::Roman> ITA_Ed_Gilbert_2 = Node::String::read<Geometry::Roman, Node::FEN_tag>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        Node::Position<Geometry::Roman> ITA_Ed_Gilbert_3 = Node::String::read<Geometry::Roman, Node::FEN_tag>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Root::perft<Variants::Italian>(ITA_Ed_Gilbert_2, 16);
        Root::perft<Variants::Italian>(ITA_Ed_Gilbert_3, 12);
        
        Node::Position<Geometry::Thai> t8 = Node::Position<Geometry::Thai>::initial();
        Root::perft<Variants::Thai>(t8, 11);      // Thai initial position with 2 rows of men
        Root::perft<Variants::Thai>(i8, 13);      // Usual initial position with 3 rows of men
}

// Test positions from the official Italian rules: http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
TEST_F(Perft, ItalianRuleBook)
{
        Node::Position<Geometry::Roman> ITA_empty = Node::String::read<Geometry::Roman, Node::FEN_tag>()("");   // Art. 2.1
        typedef Geometry::Roman ITA_notation;                                      	                        // Art. 2.4
        Node::Position<Geometry::Roman> ITA_initial;						                // Art. 2.6

        std::string ITA_rules[] = {
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

        std::cout << Node::Layout::write<Node::FEN_tag>()(ITA_empty) << std::endl;
        Geometry::Layout::write<ITA_notation, Geometry::Layout::Square_tag>()();
        std::cout << Node::Layout::write<Node::FEN_tag>()(ITA_initial) << std::endl;
        for (size_t i = 0; i < 9; ++i)
                Root::divide<Variants::Italian>(Node::String::read<Geometry::Roman, Node::FEN_tag>()(ITA_rules[i]), 1);
}

#endif

}       // namespace Search
}       // namespace Tree
