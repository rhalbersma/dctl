#include <boost/test/unit_test.hpp>
#include <iterator>
#include "../test_config.hpp"
#include "../../src/walk/Root.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/rules/Types.hpp"
#include "../../src/utility/IntegerTypes.hpp"

#if WALK_TEST == 1

namespace dctl {
namespace walk {

struct Fixture
{
        Fixture()
        {
                root_.resize_hash(24);
        }

        ~Fixture()
        {
        }

        BOOST_STATIC_CONSTANT(auto, R = 7);

        template<typename Position, std::size_t N>
        void run(Position const& p, NodeCount const (&leafs)[N])
        {
                for (auto it = std::begin(leafs); it != std::prev(std::end(leafs), R); ++it) {
                        root_.clear_hash();
                        auto const depth = std::distance(std::begin(leafs), it) + 1;
                        BOOST_CHECK_EQUAL(*it, root_.test(p, depth));
                }
        };

private:
        Root root_;
};

BOOST_AUTO_TEST_SUITE(TestPerft)

// The original perft thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308

BOOST_FIXTURE_TEST_CASE(InternationalInitial, Fixture)
{
        auto const p = Position<rules::International, board::International>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41022423, 258895763, 1665861398 };
        run(p, leafs);
}

BOOST_FIXTURE_TEST_CASE(InternationalRandom178, Fixture)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"
        );
        NodeCount const leafs[] = { 14, 55, 1168, 5432, 87195, 629010, 9041010, 86724219, 1216917193 };
        run(p, leafs);
}

BOOST_FIXTURE_TEST_CASE(InternationalWoldouby, Fixture)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        );
        NodeCount const leafs[] = { 6, 12, 30, 73, 215, 590, 1944, 6269, 22369, 88050, 377436, 1910989, 9872645, 58360286, 346184885 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(FrisianInitial, Fixture)
{
        auto const p = Position<rules::Frisian, board::International>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 3874, 21265, 102431, 540126, 2825779, 15605069, 85817725, 491186430 };
        run(p, leafs);
}

// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html

BOOST_FIXTURE_TEST_CASE(BrazilianInitial, Fixture)
{
        auto const p = Position<rules::Brazilian, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7473, 37628, 187302, 907830 , 4431766, 21560022, 105491257, 511882477, 2481546396 };
        run(p, leafs);
}

// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html

BOOST_FIXTURE_TEST_CASE(PoolInitial, Fixture)
{
        auto const p = Position<rules::Pool, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7482, 37986, 190146, 929896 , 4570534, 22435955, 110833952, 544005148, 2668385616 };
        run(p, leafs);
}

// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_FIXTURE_TEST_CASE(RussianInitial, Fixture)
{
        auto const p = Position<rules::Russian, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7482, 37986, 190146, 929899 , 4570586, 22444032, 110917189, 544770444, 2673979569 };
        run(p, leafs);
}

// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_FIXTURE_TEST_CASE(CheckersInitial, Fixture)
{
        auto const p = Position<rules::Checkers, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963680, 18391564, 85242128, 388617999, 1766564893 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(CzechInitial, Fixture)
{
        auto const p = Position<rules::Czech, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963671, 18368918, 84967210, 386262109, 1749707352 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(SpanishInitial, Fixture)
{
        auto const p = Position<rules::Spanish, board::Roman>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36473, 177532, 828783, 3860866, 17743464, 81383200, 365728331, 1637958247 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(ItalianInitial, Fixture)
{
        auto const p = Position<rules::Italian, board::Roman>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36473, 177532, 828783, 3860875, 17761384, 81647058, 367911475, 1655211086 };
        run(p, leafs);
}

/*
// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(ItalianGilbert1, Fixture)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3"
        );
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36473, 177532, 828783, 3860875, 17761384, 81647058, 367911475, 1655211086 };
        run(p, leafs);
}

BOOST_FIXTURE_TEST_CASE(ItalianGilbert2, Fixture)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "B:W30,21,22,17,20,K6:B25,28,9,5,1,3"
        );
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36473, 177532, 828783, 3860875, 17761384, 81647058, 367911475, 1655211086 };
        run(p, leafs);
}

*/

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(ThaiNative, Fixture)
{
        auto const p = Position<rules::Thai, board::Thai>::initial();
        NodeCount const leafs[] = { 7, 49, 392, 3136, 26592, 218695, 1820189, 14533014, 114530830, 861842812, 6304986761 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(ThaiCheckers, Fixture)
{
        auto const p = Position<rules::Thai, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963648, 18363523, 84892793, 385713660, 1745666630 };
        run(p, leafs);
}

// The rectangular board thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014

BOOST_FIXTURE_TEST_CASE(Ktar12Initial, Fixture)
{
        auto const p = Position<rules::International, board::Ktar12>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41291394 };
        run(p, leafs);
}

/*
BOOST_AUTO_TEST_CASE(Rectangular)
{
        std::cout << setup::diagram<board::Spantsireti>()();

        auto iS = Position<board::Spantsireti>::initial();
        root_.clear_hash();
        root_.perft<rules::International>(iS, 9);

        std::cout << setup::diagram<board::Ktar11>()();

        auto iK11 = Position<board::Ktar11>::initial();
        root_.clear_hash();
        root_.perft<rules::International>(iK11, 9);
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl

#endif
