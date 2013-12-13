#include <iostream>
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/ray/cursor.hpp>
#include <dctl/ray/iterator.hpp>
#include <dctl/ray/transform.hpp>
#include <dctl/board/types.hpp>
#include <dctl/angle.hpp>

namespace dctl {
namespace ray {

BOOST_AUTO_TEST_SUITE(RayCursor)

template<class Board, int N>
int fun(Iterator<Board, N> const& x)
{
        return *x;
}

using B = board::International;
static constexpr auto dir = 135_deg;

BOOST_AUTO_TEST_CASE(Members)
{
        auto c = StridedCursor<B, dir>(0);
        for (auto i = 0; i < 5; ++i) {
                ++c;
                std::cout << c << std::endl;
        }

        constexpr auto stride = B::shift_size(dir);
        std::cout << "stride: " << stride << '\n';
        auto it = Iterator<board::International, dir>{c};
        auto rt = rotate<+135_deg>(it);

        for (auto i = 0; i < 5; ++i)
                std::cout << fun(rt++) << '\n';
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace ray
}       // namespace dctl
