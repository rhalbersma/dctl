#include <test/config.hpp>

#if WALK_INITIAL_BOARD8x8 == 1

#include <boost/test/unit_test.hpp>
#include <test/walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/node/position.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/types.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(TestInitialBoard8x8)
        /*
// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html

BOOST_FIXTURE_TEST_CASE(brazilianInitial, Fixture)
{
        auto const p = Position<rules::Brazilian, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7473, 37628, 187302, 907830 , 4431766, 21560022, 105491257, 511882477, 2481546396LL };
        run(p, leafs);
}

// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html

BOOST_FIXTURE_TEST_CASE(poolInitial, Fixture)
{
        auto const p = Position<rules::Pool, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7482, 37986, 190146, 929896 , 4570534, 22435955, 110833952, 544005148, 2668385616LL };
        run(p, leafs);
}

// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_FIXTURE_TEST_CASE(russianInitial, Fixture)
{
        auto const p = Position<rules::Russian, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7482, 37986, 190146, 929899 , 4570586, 22444032, 110917189, 544770444, 2673979569LL };
        run(p, leafs);
}
*/
// The alternative game rules thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

typedef Fixture< Position<rules::Checkers, board::Checkers> > CheckersFixture;
BOOST_FIXTURE_TEST_CASE(checkersInitial, CheckersFixture)
{
        auto const p = Position<rules::Checkers, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963680, 18391564, 85242128, 388617999, 1766564893 };
        run(p, leafs);
}
/*
// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(czechInitial, Fixture)
{
        auto const p = Position<rules::Czech, board::Checkers>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36768, 179740, 845931, 3963671, 18368918, 84967210, 386262109, 1749707352 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(spanishInitial, Fixture)
{
        auto const p = Position<rules::Spanish, board::Roman>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36473, 177532, 828783, 3860866, 17743464, 81383200, 365728331, 1637958247 };
        run(p, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_FIXTURE_TEST_CASE(italianInitial, Fixture)
{
        auto const p = Position<rules::Italian, board::Roman>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7361, 36473, 177532, 828783, 3860875, 17761384, 81647058, 367911475, 1655211086 };
        run(p, leafs);
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl

#endif
