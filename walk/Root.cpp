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

// The original perft thread on the FMJD forum 
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308

typedef Fixture<variant::International, board::International> FixtureInternational;

BOOST_FIXTURE_TEST_CASE(InternationalInitial, FixtureInternational)
{
        std::cout << setup::diagram<board::International>()();

        const auto p = Position<board::International>::initial();
        const NodeCount leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41022423, 258895763, 1665861398 };
        for (auto i = 0; i < 11; ++i)
                run(p, i + 1, leafs[i]);
}

BOOST_FIXTURE_TEST_CASE(InternationalRandom178, FixtureInternational)
{
        const auto p(setup::read<board::International, pdn::protocol>()(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"
        ));
        const NodeCount leafs[] = { 14, 55, 1168, 5432, 87195, 629010, 9041010, 86724219, 1216917193 };
        for (auto i = 0; i < 9; ++i)
                run(p, i + 1, leafs[i]);
}

BOOST_FIXTURE_TEST_CASE(InternationalWoldouby, FixtureInternational)
{        
        const auto p(setup::read<board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        ));
        const NodeCount leafs[] = { 6, 12, 30, 73, 215, 590, 1944, 6269, 22369, 88050, 377436, 1910989, 9872645, 58360286, 346184885 };
        for (auto i = 0; i < 15; ++i)
                run(p, i + 1, leafs[i]);
}

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

// http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
typedef Fixture<variant::Brazilian, board::Checkers> FixtureBrazilian;
BOOST_FIXTURE_TEST_CASE(BrazilianInitial, FixtureBrazilian)
{
        const auto p = Position<board::Checkers>::initial();
        const NodeCount leafs[] = { 7, 49, 302, 1469, 7473, 37628, 187302, 907830 , 4431766, 21560022, 105491257, 511882477, 2481546396 };
        for (auto i = 0; i < 13; ++i)
                run(p, i + 1, leafs[i]);
}

// http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
typedef Fixture<variant::Pool, board::Checkers> FixturePool;
BOOST_FIXTURE_TEST_CASE(PoolInitial, FixturePool)
{
        const auto p = Position<board::Checkers>::initial();
        const NodeCount leafs[] = { 7, 49, 302, 1469, 7482, 37986, 190146, 929896 , 4570534, 22435955, 110833952, 544005148, 2668385616 };
        for (auto i = 0; i < 13; ++i)
                run(p, i + 1, leafs[i]);
}

// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
typedef Fixture<variant::Russian, board::Checkers> FixtureRussian;
BOOST_FIXTURE_TEST_CASE(RussianInitial, FixtureRussian)
{
        const auto p = Position<board::Checkers>::initial();
        const NodeCount leafs[] = { 7, 49, 302, 1469, 7482, 37986, 190146, 929899 , 4570586, 22444032, 110917189, 544770444, 2673979569 };
        for (auto i = 0; i < 13; ++i)
                run(p, i + 1, leafs[i]);
}

// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
typedef Fixture<variant::Checkers, board::Checkers> FixtureCheckers;
BOOST_FIXTURE_TEST_CASE(CheckersInitial, FixtureCheckers)
{
        const auto p = Position<board::Checkers>::initial();
        const NodeCount leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963680, 18391564, 85242128, 388617999, 1766564893 };
        for (auto i = 0; i < 13; ++i)
                run(p, i + 1, leafs[i]);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
typedef Fixture<variant::Thai, board::Thai> FixtureThaiNative;
BOOST_FIXTURE_TEST_CASE(ThaiNative, FixtureThaiNative)
{
        const auto p = Position<board::Thai>::initial();
        const NodeCount leafs[] = { 7, 49, 392, 3136, 26592, 218695, 1820189, 14533014, 114530830, 861842812, 6304986761 };
        for (auto i = 0; i < 11; ++i)
                run(p, i + 1, leafs[i]);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
typedef Fixture<variant::Thai, board::Checkers> FixtureThaiCheckers;
BOOST_FIXTURE_TEST_CASE(ThaiCheckers, FixtureThaiCheckers)
{
        const auto p = Position<board::Checkers>::initial();
        const NodeCount leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963648, 18363523, 84892793, 385713660, 1745666630 };
        for (auto i = 0; i < 13; ++i)
                run(p, i + 1, leafs[i]);
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
