#include <dctl/board/mask.hpp>                  // Column, JumpGroup, Row, Squares
#include <dctl/set_type.hpp>                    // set_type
#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsireti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact1012, Compact120, Board<12, 10>, Canadian, SriLankan, Dumm
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                            // all_of
#include <numeric>                              // accumulate

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(SquaresSizeEqualsBoardSize, T, BoardSequence)
{
        BOOST_CHECK_EQUAL(Squares<T>::mask().count(), T::size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColumnsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<int>{0};
        auto const omega = boost::counting_iterator<int>{T::width()};

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return Column<T>::mask(Color::black, i) == Column<T>::mask(Color::white, T::width() - 1 - i);
                })
        );

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return std::all_of(alpha, omega, [=](auto j){
                                return i == j ? true : disjoint(Column<T>::mask(Color::white, i), Column<T>::mask(Color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(alpha, omega, set_type<T>{}, [](auto result, auto i){
                        return result ^ Column<T>::mask(Color::white, i);
                }) == Squares<T>::mask()
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RowsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<int>{0};
        auto const omega = boost::counting_iterator<int>{T::height()};

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return Row<T>::mask(Color::black, i) == Row<T>::mask(Color::white, T::height() - 1 - i);
                })
        );

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return std::all_of(alpha, omega, [=](auto j){
                                return i == j ? true : disjoint(Row<T>::mask(Color::white, i), Row<T>::mask(Color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(alpha, omega, set_type<T>{}, [](auto result, auto i){
                        return result ^ Row<T>::mask(Color::white, i);
                }) == Squares<T>::mask()
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpGroupsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const alpha = boost::counting_iterator<std::size_t>{0};
        auto const omega = boost::counting_iterator<std::size_t>{4};

        BOOST_CHECK(
                std::all_of(alpha, omega, [=](auto i){
                        return std::all_of(alpha, omega, [=](auto j){
                                return i == j ? true : disjoint(JumpGroup<T>::mask(i), JumpGroup<T>::mask(j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(alpha, omega, set_type<T>{}, [](auto result, auto i){
                        return result ^ JumpGroup<T>::mask(i);
                }) == Squares<T>::mask()
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
