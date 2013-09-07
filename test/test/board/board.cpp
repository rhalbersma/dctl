#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/types.hpp>

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        International,
        Roman,
        Frisian,
        Spantsireti,
        Ktar11,
        Ktar12
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Bit2SquareIsInverseSquare2Bit, T, BoardSequence)
{
        for (auto sq = T::begin(); sq != T::end(); ++sq)
                BOOST_CHECK_EQUAL(T::bit2square(T::square2bit(sq)), sq);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
