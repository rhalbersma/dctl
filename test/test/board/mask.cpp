#include <algorithm>                            // accumulate
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/types.hpp>
#include <dctl/board/mask/jump_group.hpp>
#include <dctl/board/mask/squares.hpp>

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

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpGroupsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<int>{0};
        auto const omega = boost::counting_iterator<int>{4};

        BOOST_CHECK(board::Squares<T>::mask() == std::accumulate(alpha, omega, typename T::set_type{}, [](typename T::set_type result, auto i){
                return result ^ board::JumpGroup<T>::mask(i);
        }));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
