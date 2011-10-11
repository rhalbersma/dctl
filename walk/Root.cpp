#include <boost/test/unit_test.hpp> 
#include "../test_config.h"
#include "../../src/walk/Root.h"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/variant/Variant.hpp"
#include "../../src/utility/IntegerTypes.hpp"

namespace dctl {
namespace walk {

#if INTEGRATION_TEST == 1

struct FixtureHashTable
{
        FixtureHashTable() 
        {
                Root::resize_hash(24);
        }

        ~FixtureHashTable() 
        {
                Root::resize_hash(0);
        }
};

template<typename Rules>
struct Run 
{
        template<typename Board>
        void operator()(const Position<Board>& position, int depth, NodeCount leafs) const
        {
                BOOST_CHECK_EQUAL(leafs, Root::test<Rules>(position, depth));
        }
};

BOOST_FIXTURE_TEST_SUITE(TestPerft, FixtureHashTable)

// The original perft thread on the FMJD forum 
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308
BOOST_AUTO_TEST_CASE(International)
{
        std::cout << setup::diagram<board::International>()();

        auto i10 = Position<board::International>::initial();
        Root::clear_hash();
        NodeCount leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41022423, 258895763, 1665861398 };
        for (auto i = 0; i < 11; ++i)
                Run<variant::International>()(i10, i + 1, leafs[i]);

        auto random178(setup::read<board::International, pdn::protocol>()(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46")
        );
        Root::clear_hash();
        Root::perft<variant::International>(random178, 9);

        auto Woldouby(setup::read<board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38")
        );
        Root::clear_hash();
        Root::perft<variant::International>(Woldouby, 15);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
BOOST_AUTO_TEST_CASE(Frisian)
{
        std::cout << setup::diagram<board::Frisian>()();

        auto f10 = Position<board::Frisian>::initial();
        Root::clear_hash();
        Root::perft<variant::Frisian>(f10, 11);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
BOOST_AUTO_TEST_CASE(ChessVariants)
{
        std::cout << setup::diagram<board::Checkers>()();
        auto i8 = Position<board::Checkers>::initial();
/*        
        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Root::clear_hash();
        Root::perft<variant::Brazilian>(i8, 13); 

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Root::clear_hash();
        Root::perft<variant::Pool>(i8, 13);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::clear_hash();
        Root::perft<variant::Russian>(i8, 14);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::clear_hash();
        Root::perft<variant::English>(i8, 14);   

        Root::clear_hash();
        Root::perft<variant::Czech>(i8, 13);

        std::cout << setup::diagram<board::Roman>()();
        auto r8 = Position<board::Roman>::initial();

        Root::clear_hash();
        Root::perft<variant::Spanish>(r8, 13);

        Root::clear_hash();
        Root::perft<variant::Italian>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        auto ITA_Ed_Gilbert_1 = setup::read<board::Roman, pdn::protocol>()(
                "W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3"
        );
        auto ITA_Ed_Gilbert_2 = setup::read<board::Roman, pdn::protocol>()(
                "B:W30,21,22,17,20,K6:B25,28,9,5,1,3"
        );
        Root::clear_hash();
        Root::perft<variant::Italian>(ITA_Ed_Gilbert_1, 16);
        Root::clear_hash();
        Root::perft<variant::Italian>(ITA_Ed_Gilbert_2, 12);
        */
        auto t8 = Position<board::Thai>::initial();
        Root::clear_hash();
        Root::perft<variant::Thai>(t8, 11);      // Thai initial position with 2 rows of men
        Root::clear_hash();
        Root::perft<variant::Thai>(i8, 13);      // Usual initial position with 3 rows of men
}

// The rectangular board thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
BOOST_AUTO_TEST_CASE(Rectangular)
{
        std::cout << setup::diagram<board::Spantsireti>()();

        auto iS = Position<board::Spantsireti>::initial();
        Root::clear_hash();
        Root::perft<variant::International>(iS, 9);

        std::cout << setup::diagram<board::Ktar11>()();
        
        auto iK11 = Position<board::Ktar11>::initial();
        Root::clear_hash();
        Root::perft<variant::International>(iK11, 9);
                
        std::cout << setup::diagram<board::Ktar12>()();
        
        auto iK12 = Position<board::Ktar12>::initial();
        Root::clear_hash();
        Root::perft<variant::International>(iK12, 9);
}

BOOST_AUTO_TEST_SUITE_END()

#endif

}       // namespace walk
}       // namespace dctl
