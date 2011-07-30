#include "gtest/gtest.h"
#include "../test_config.h"
#include "../../../Library/src/walk/Root.h"
#include "../../../Library/src/node/Position.h"
#include "../../../Library/src/board/Types.h"
#include "../../../Library/src/rules/Variants.h"

#include "../../../Library/src/setup/Diagram.h"
#include "../../../Library/src/setup/String.h"
#include "../../../Library/src/protocol/pdn/PDN.h"

namespace dctl {
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

#if INTEGRATION_TEST == 0

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
TEST_F(Perft, Frisian)
{
        std::cout << setup::diagram<board::Frisian, setup::squares>()() << std::endl;
        std::cout << setup::diagram<board::Frisian, setup::bits>()() << std::endl;

        auto f10 = Position<board::Frisian>::initial();
        Root::clear_hash();
        Root::perft<rules::Frisian>(f10, 11);
}

// The original perft thread on the FMJD forum 
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308
TEST_F(Perft, International)
{
        std::cout << setup::diagram<board::International, setup::squares>()() << std::endl;
        std::cout << setup::diagram<board::International, setup::bits>()() << std::endl;

        auto i10 = Position<board::International>::initial();
        Root::clear_hash();
        Root::perft<rules::International>(i10, 11);

        auto random178(setup::read<board::International, protocol::pdn::version>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Root::clear_hash();
        Root::perft<rules::International>(random178, 9);

        auto Woldouby(setup::read<board::International, protocol::pdn::version>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        Root::clear_hash();
        Root::perft<rules::International>(Woldouby, 15);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
TEST_F(Perft, ChessVariants)
{
        auto i8 = Position<board::Checkers>::initial();
        std::cout << setup::diagram<board::Checkers, setup::squares>()() << std::endl;
        
        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Root::clear_hash();
        Root::perft<rules::Brazilian>(i8, 13); 

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Root::clear_hash();
        Root::perft<rules::Pool>(i8, 13);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::clear_hash();
        Root::perft<rules::Russian>(i8, 14);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::clear_hash();
        Root::perft<rules::English>(i8, 14);   

        Root::clear_hash();
        Root::perft<rules::Czech>(i8, 13);

        auto r8 = Position<board::Roman>::initial();
        std::cout << setup::diagram<board::Roman, setup::squares>()() << std::endl;

        Root::clear_hash();
        Root::perft<rules::Spanish>(r8, 13);

        Root::clear_hash();
        Root::perft<rules::Italian>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        auto ITA_Ed_Gilbert_1 = setup::read<board::Roman, protocol::pdn::version>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        auto ITA_Ed_Gilbert_2 = setup::read<board::Roman, protocol::pdn::version>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Root::clear_hash();
        Root::perft<rules::Italian>(ITA_Ed_Gilbert_1, 16);
        Root::clear_hash();
        Root::perft<rules::Italian>(ITA_Ed_Gilbert_2, 12);
        
        auto t8 = Position<board::Thai>::initial();
        Root::clear_hash();
        Root::perft<rules::Thai>(t8, 11);      // Thai initial position with 2 rows of men
        Root::clear_hash();
        Root::perft<rules::Thai>(i8, 13);      // Usual initial position with 3 rows of men
}

// The rectangular board thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
TEST_F(Perft, Rectangular)
{
        std::cout << setup::diagram<board::Spantsireti, setup::squares>()() << std::endl;
        std::cout << setup::diagram<board::Spantsireti, setup::bits>()() << std::endl;

        auto iS = Position<board::Spantsireti>::initial();
        Root::clear_hash();
        Root::perft<rules::International>(iS, 9);

        std::cout << setup::diagram<board::Ktar11, setup::squares>()() << std::endl;
        std::cout << setup::diagram<board::Ktar11, setup::bits>()() << std::endl;
        
        auto iK11 = Position<board::Ktar11>::initial();
        Root::clear_hash();
        Root::perft<rules::International>(iK11, 9);
                
        std::cout << setup::diagram<board::Ktar12, setup::squares>()() << std::endl;
        std::cout << setup::diagram<board::Ktar12, setup::bits>()() << std::endl;
        
        auto iK12 = Position<board::Ktar12>::initial();
        Root::clear_hash();
        Root::perft<rules::International>(iK12, 9);
}

#endif

}       // namespace walk
}       // namespace dctl
