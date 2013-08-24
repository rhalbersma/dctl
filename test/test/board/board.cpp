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

BOOST_AUTO_TEST_CASE_TEMPLATE(SquareBitConversion, T, BoardSequence)
{
        for (auto sq = T::begin(); sq != T::end(); ++sq) {
                auto b = T::square2bit(sq);
                auto s = T::bit2square(b);
                BOOST_CHECK_EQUAL(s, sq);
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
