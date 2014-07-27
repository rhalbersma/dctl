#include <dctl/bit.hpp>                         // set_intersects
#include <dctl/board/mask.hpp>                  // Column, JumpGroup, Row, Squares
#include <dctl/board/types.hpp>                 // Micro, Mini, Checkers, International, Roman, Frisian, Spantsireti, Ktar11, Ktar12
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                            // accumulate, all_of

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        Roman,
        Spantsireti,
        International,
        Frisian,
        Ktar<10, 11>,
        Ktar<10, 12>,
        Compact1012,
        Compact1210,
        Board<12, 10>,
        Canadian,
        SriLankan,
        Dumm
>;

template<class T>
using Set = typename T::set_type;

BOOST_AUTO_TEST_CASE_TEMPLATE(SquaresSizeEqualsBoardSize, T, BoardSequence)
{
        BOOST_CHECK_EQUAL(Squares<T>::mask().size(), T::size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColumnsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<int>{0};
        auto const omega = boost::counting_iterator<int>{T::width};

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return Column<T>::mask(Color::black, i) == Column<T>::mask(Color::white, T::width - 1 - i);
                })
        );

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return std::all_of(alpha, omega, [=](auto j){
                                return i == j ? true : !set_intersects(Column<T>::mask(Color::white, i), Column<T>::mask(Color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(alpha, omega, Set<T>{}, [](auto result, auto i){
                        return result ^ Column<T>::mask(Color::white, i);
                }) == Squares<T>::mask()
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RowsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<int>{0};
        auto const omega = boost::counting_iterator<int>{T::height};

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return Row<T>::mask(Color::black, i) == Row<T>::mask(Color::white, T::height - 1 - i);
                })
        );

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return std::all_of(alpha, omega, [=](auto j){
                                return i == j ? true : !set_intersects(Row<T>::mask(Color::white, i), Row<T>::mask(Color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(alpha, omega, Set<T>{}, [](auto result, auto i){
                        return result ^ Row<T>::mask(Color::white, i);
                }) == Squares<T>::mask()
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpGroupsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<int>{0};
        auto const omega = boost::counting_iterator<int>{4};

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return std::all_of(alpha, omega, [=](auto j){
                                return i == j ? true : !set_intersects(JumpGroup<T>::mask(i), JumpGroup<T>::mask(j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(alpha, omega, Set<T>{}, [](auto result, auto i){
                        return result ^ JumpGroup<T>::mask(i);
                }) == Squares<T>::mask()
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
