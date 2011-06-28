#include "gtest/gtest.h"
#include "../../test_config.h"
#include "../../../../Library/Source/Tree/Walk/Root.h"
#include "../../../../Library/Source/Tree/Node/Position.h"
#include "../../../../Library/Source/Tree/Node/Protocol.h"
#include "../../../../Library/Source/Tree/Node/String.h"
#include "../../../../Library/Source/Geometry/Board.h"
#include "../../../../Library/Source/Geometry/Layout.h"
#include "../../../../Library/Source/Variants/Rules.h"

namespace tree {
namespace walk {

// The fixture for testing class SearchEndgame.
class Perft: public ::testing::Test {
protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Perft() {
        // You can do set-up work for each test here.
                Root::resize_hash(24);
        }

        virtual ~Perft() {
        // You can do clean-up work that doesn't throw exceptions here.
                Root::resize_hash(0);
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

#if INTEGRATION_TEST == 1

// The original perft thread on the FMJD forum 
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308
TEST_F(Perft, International)
{
        std::cout << geometry::layout::write<geometry::International, geometry::layout::Square_tag>()() << std::endl;
        std::cout << geometry::layout::write<geometry::International, geometry::layout::Bit_tag>()() << std::endl;

        node::Position<geometry::International> i10 = node::Position<geometry::International>::initial();
        Root::clear_hash();
        Root::perft<variants::International>(i10, 11);

        node::Position<geometry::International> random178(node::string::read<geometry::International, node::FEN_tag>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Root::clear_hash();
        Root::perft<variants::International>(random178, 9);

        node::Position<geometry::International> Woldouby(node::string::read<geometry::International, node::FEN_tag>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        Root::clear_hash();
        Root::perft<variants::International>(Woldouby, 15);
}

// The rectangular board thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
TEST_F(Perft, Rectangular)
{
        std::cout << geometry::layout::write<geometry::Spantsireti, geometry::layout::Square_tag>()() << std::endl;
        std::cout << geometry::layout::write<geometry::Spantsireti, geometry::layout::Bit_tag>()() << std::endl;

        node::Position<geometry::Spantsireti> iS = node::Position<geometry::Spantsireti>::initial();
        Root::clear_hash();
        Root::perft<variants::International>(iS, 9);

        std::cout << geometry::layout::write<geometry::Ktar11, geometry::layout::Square_tag>()() << std::endl;
        std::cout << geometry::layout::write<geometry::Ktar11, geometry::layout::Bit_tag>()() << std::endl;
        
        node::Position<geometry::Ktar11> iK11 = node::Position<geometry::Ktar11>::initial();
        Root::clear_hash();
        Root::perft<variants::International>(iK11, 9);
                
        std::cout << geometry::layout::write<geometry::Ktar12, geometry::layout::Square_tag>()() << std::endl;
        std::cout << geometry::layout::write<geometry::Ktar12, geometry::layout::Bit_tag>()() << std::endl;
        
        node::Position<geometry::Ktar12> iK12 = node::Position<geometry::Ktar12>::initial();
        Root::clear_hash();
        Root::perft<variants::International>(iK12, 9);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
TEST_F(Perft, ChessVariants)
{
        node::Position<geometry::Chess> i8 = node::Position<geometry::Chess>::initial();
        std::cout << geometry::layout::write<geometry::Chess, geometry::layout::Square_tag>()() << std::endl;
        
        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Root::clear_hash();
        Root::perft<variants::Brazilian>(i8, 13); 

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Root::clear_hash();
        Root::perft<variants::Pool>(i8, 13);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::clear_hash();
        Root::perft<variants::Russian>(i8, 14);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::clear_hash();
        Root::perft<variants::English>(i8, 14);   

        Root::clear_hash();
        Root::perft<variants::Czech>(i8, 13);

        node::Position<geometry::Roman> r8 = node::Position<geometry::Roman>::initial();
        std::cout << geometry::layout::write<geometry::Roman, geometry::layout::Square_tag>()() << std::endl;

        Root::clear_hash();
        Root::perft<variants::Spanish>(r8, 13);

        Root::clear_hash();
        Root::perft<variants::Italian>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        node::Position<geometry::Roman> ITA_Ed_Gilbert_2 = node::string::read<geometry::Roman, node::FEN_tag>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        node::Position<geometry::Roman> ITA_Ed_Gilbert_3 = node::string::read<geometry::Roman, node::FEN_tag>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Root::clear_hash();
        Root::perft<variants::Italian>(ITA_Ed_Gilbert_2, 16);
        Root::clear_hash();
        Root::perft<variants::Italian>(ITA_Ed_Gilbert_3, 12);
        
        node::Position<geometry::Thai> t8 = node::Position<geometry::Thai>::initial();
        Root::clear_hash();
        Root::perft<variants::Thai>(t8, 11);      // Thai initial position with 2 rows of men
        Root::clear_hash();
        Root::perft<variants::Thai>(i8, 13);      // Usual initial position with 3 rows of men
}

#endif

}       // namespace walk
}       // namespace tree
