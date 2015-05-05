#include <walk/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/position.hpp>
#include <dctl/rules.hpp>
#include <dctl/walk/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>                      // size_t

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialBoard8x8)

// The "Standard validation generator moves" thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html

BOOST_AUTO_TEST_CASE(BrazilianInitial)
{
        auto const p = Position<rules::Brazilian, board::Checkers>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'473, 37'628, 187'302, 907'830 , 4'431'766, 21'560'022, 105'491'257, 511'882'477, 2'481'546'396 };
        test(p, unique_gen, leafs);
}

// The "Standard validation generator moves" thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html

BOOST_AUTO_TEST_CASE(PoolInitial)
{
        auto const p = Position<rules::Pool, board::Checkers>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'482, 37'986, 190'146, 929'896 , 4'570'534, 2'2435'955, 110'833'952, 544'005'148, 2'668'385'616 };
        test(p, unique_gen, leafs);
}

// The "Standard validation generator moves" thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_AUTO_TEST_CASE(RussianInitial)
{
        auto const p = Position<rules::Russian, board::Checkers>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'482, 37'986, 190'146, 929'899 , 4'570'586, 22'444'032, 110'917'189, 544'770'444, 2'673'979'569 };
        test(p, unique_gen, leafs);
}

// The "Standard validation generator moves" thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_AUTO_TEST_CASE(CheckersInitial)
{
        auto const p = Position<rules::Checkers, board::Checkers>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'680, 18'391'564, 85'242'128, 388'617'999, 1'766'564'893 };
        test(p, unique_gen, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(CzechInitial)
{
        auto const p = Position<rules::Czech, board::Checkers>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'671, 18'368'918, 84'967'210, 386'262'109, 1'749'707'352 };
        test(p, unique_gen, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(SpanishInitial)
{
        auto const p = Position<rules::Spanish, board::Roman>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'361, 36'473, 177'532, 828'783, 3'860'866, 17'743'464, 81'383'200, 365'728'331, 1'637'958'247 };
        test(p, unique_gen, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(ItalianInitial)
{
        auto const p = Position<rules::Italian, board::Roman>::initial();
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'361, 36'473, 177'532, 828'783, 3'860'875, 17'761'384, 81'647'058, 367'911'475, 1'655'211'086 };
        test(p, unique_gen, leafs);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(ThaiInitial)
{
        auto const p = Position<rules::Thai, board::Checkers>::initial(2);
        std::size_t const leafs[] = { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'648, 18'363'523, 84'892'793, 385'713'660, 1'745'666'630 };
        test(p, unique_gen, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
