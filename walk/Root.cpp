#include <boost/test/unit_test.hpp> 
#include "../test_config.hpp"
#include "../../src/walk/Root.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/variant/Variant.hpp"
#include "../../src/utility/IntegerTypes.hpp"

namespace dctl {
namespace walk {

#if INTEGRATION_TEST == 0

template<typename Rules, typename Board>
struct Fixture
{
        Fixture() 
        {
                root_.resize_hash(24);
        }

        ~Fixture() 
        {
        }

        void run(const Position<Board>& position, int depth, NodeCount leafs) 
        {
                root_.clear_hash();
                BOOST_CHECK_EQUAL(leafs, root_.test(position, depth));
        };

        Root<Rules, Board> root_;
};

BOOST_AUTO_TEST_SUITE(TestPerft)

/*
// The original perft thread on the FMJD forum 
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308

typedef Fixture<variant::International, board::International> FixtureInternational;

BOOST_FIXTURE_TEST_CASE(InternationalInitial, FixtureInternational)
{
        std::cout << setup::diagram<board::International>()();

        auto p = Position<board::International>::initial();
        NodeCount leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41022423, 258895763, 1665861398 };
        for (auto i = 0; i < 11; ++i)
                run(p, i + 1, leafs[i]);
}

BOOST_FIXTURE_TEST_CASE(InternationalRandom178, FixtureInternational)
{
        auto p(setup::read<board::International, pdn::protocol>()(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46")
        );
        NodeCount leafs[] = { 14, 55, 1168, 5432, 87195, 629010, 9041010, 86724219, 1216917193 };
        for (auto i = 0; i < 9; ++i)
                run(p, i + 1, leafs[i]);
}

BOOST_FIXTURE_TEST_CASE(InternationalWoldouby, FixtureInternational)
{        
        auto p(setup::read<board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38")
        );
        NodeCount leafs[] = { 6, 12, 30, 73, 215, 590, 1944, 6269, 22369, 88050, 377436, 1910989, 9872645, 58360286, 346184885 };
        for (auto i = 0; i < 15; ++i)
                run(p, i + 1, leafs[i]);
}
*/
/*
typedef Fixture<variant::Frisian, board::Frisian> FixtureFrisian;

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
BOOST_FIXTURE_TEST_CASE(Frisian, FixtureFrisian)
{
        std::cout << setup::diagram<board::Frisian>()();

        auto f10 = Position<board::Frisian>::initial();
        root_.clear_hash();
        root_.perft(f10, 11);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
BOOST_AUTO_TEST_CASE(ChessVariants)
{
        std::cout << setup::diagram<board::Checkers>()();
        auto i8 = Position<board::Checkers>::initial();

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        root_.clear_hash();
        root_.perft<variant::Brazilian>(i8, 13); 

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        root_.clear_hash();
        root_.perft<variant::Pool>(i8, 13);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        root_.clear_hash();
        root_.perft<variant::Russian>(i8, 14);

        // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        root_.clear_hash();
        root_.perft<variant::English>(i8, 14);   

        root_.clear_hash();
        root_.perft<variant::Czech>(i8, 13);

        std::cout << setup::diagram<board::Roman>()();
        auto r8 = Position<board::Roman>::initial();

        root_.clear_hash();
        root_.perft<variant::Spanish>(r8, 13);

        root_.clear_hash();
        root_.perft<variant::Italian>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        auto ITA_Ed_Gilbert_1 = setup::read<board::Roman, pdn::protocol>()(
                "W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3"
        );
        auto ITA_Ed_Gilbert_2 = setup::read<board::Roman, pdn::protocol>()(
                "B:W30,21,22,17,20,K6:B25,28,9,5,1,3"
        );
        root_.clear_hash();
        root_.perft<variant::Italian>(ITA_Ed_Gilbert_1, 16);
        root_.clear_hash();
        root_.perft<variant::Italian>(ITA_Ed_Gilbert_2, 12);

        root_.clear_hash();
        root_.perft<variant::Thai>(i8, 13);      // Usual initial position with 3 rows of men
}
*/
typedef Fixture<variant::Thai, board::Thai> FixtureThaiNative;

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
BOOST_FIXTURE_TEST_CASE(ThaiNative, FixtureThaiNative)
{
        std::cout << setup::diagram<board::Thai>()();
        auto p = Position<board::Thai>::initial();
        root_.clear_hash();
        root_.perft(p, 11);      // Thai initial position with 2 rows of men
}

typedef Fixture<variant::Thai, board::Checkers> FixtureThaiCheckers;

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
BOOST_FIXTURE_TEST_CASE(ThaiCheckers, FixtureThaiCheckers)
{
        std::cout << setup::diagram<board::Checkers>()();
        auto p = Position<board::Checkers>::initial();
        root_.clear_hash();
        root_.perft(p, 13);      // Usual initial position with 3 rows of men
}

/*
// The rectangular board thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
BOOST_AUTO_TEST_CASE(Rectangular)
{
        std::cout << setup::diagram<board::Spantsireti>()();

        auto iS = Position<board::Spantsireti>::initial();
        root_.clear_hash();
        root_.perft<variant::International>(iS, 9);

        std::cout << setup::diagram<board::Ktar11>()();
        
        auto iK11 = Position<board::Ktar11>::initial();
        root_.clear_hash();
        root_.perft<variant::International>(iK11, 9);
                
        std::cout << setup::diagram<board::Ktar12>()();
        
        auto iK12 = Position<board::Ktar12>::initial();
        root_.clear_hash();
        root_.perft<variant::International>(iK12, 9);
}
*/
BOOST_AUTO_TEST_SUITE_END()

#endif

}       // namespace walk
}       // namespace dctl
