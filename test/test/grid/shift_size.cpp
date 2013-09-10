#include <algorithm>                            // any_of
#include <iterator>                             // begin, end
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform.hpp>              // transform
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>             // rotate
#include <dctl/board/types.hpp>                 // Micro, Mini, Checkers, International, Roman, Frisian, Spantsireti, Ktar11, Ktar12
#include <dctl/grid/shift_size.hpp>             // shift_size

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestShiftSize)

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

template<class Board>
struct make_grid
{
        using type = typename Board::InternalGrid;
};

using GridSequence = boost::mpl::transform<
        BoardSequence,
        make_grid< boost::mpl::_1 >
>::type;

int const directions[] =
{
         angle::D000, angle::D045, angle::D090, angle::D135,
         angle::D180, angle::D225, angle::D270, angle::D315
};

BOOST_AUTO_TEST_CASE_TEMPLATE(RotateAngle180SymmetryForAllDirections, T, GridSequence)
{
        BOOST_CHECK(
                std::all_of(std::begin(directions), std::end(directions), [](int const& direction) {
                        return grid::shift_size<T>(direction) == grid::shift_size<T>(angle::rotate(direction, angle::D180));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
