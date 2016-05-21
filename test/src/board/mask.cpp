#include <dctl/board/mask.hpp>                  // Column, JumpGroup, Row, Squares
#include <dctl/utility/type_traits.hpp>         // set_t
#include <board/sequence.hpp>                   // micro, mini, checkers, roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, compact_10_12, compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <xstd/cstddef.hpp>                     // _z
#include <range/v3/all.hpp>                     // all_of, view::iota
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>
#include <numeric>
#include <vector>

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(SquaresSizeEqualsBoardSize, T, BoardSequence)
{
        BOOST_CHECK_EQUAL(squares_v<T>.count(), T::size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColumnsEquivalencePartitionSquares, T, BoardSequence)
{
        using namespace xstd::support_literals;
        auto const columns = ranges::view::iota(0_zu, T::width);

        BOOST_CHECK(
                ranges::all_of(columns, [=](auto i){
                        return Column<T>::mask(Color::black, i) == Column<T>::mask(Color::white, T::width - 1 - i);
                })
        );

        BOOST_CHECK(
                ranges::all_of(columns, [=](auto i){
                        return ranges::all_of(columns, [=](auto j){
                                return i == j ? true : disjoint(Column<T>::mask(Color::white, i), Column<T>::mask(Color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                ranges::accumulate(columns, set_t<T>{}, [](auto result, auto i){
                        return result ^ Column<T>::mask(Color::white, i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RowsEquivalencePartitionSquares, T, BoardSequence)
{
        using namespace xstd::support_literals;
        auto const rows = ranges::view::iota(0_zu, T::height);

        BOOST_CHECK(
                ranges::all_of(rows, [=](auto i){
                        return Row<T>::mask(Color::black, i) == Row<T>::mask(Color::white, T::height - 1 - i);
                })
        );

        BOOST_CHECK(
                ranges::all_of(rows, [=](auto i){
                        return ranges::all_of(rows, [=](auto j){
                                return i == j ? true : disjoint(Row<T>::mask(Color::white, i), Row<T>::mask(Color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                ranges::accumulate(rows, set_t<T>{}, [](auto result, auto i){
                        return result ^ Row<T>::mask(Color::white, i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpGroupsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const jump_groups = std::vector<std::size_t>{ 0, 1, 2, 3 };

        BOOST_CHECK(
                std::all_of(jump_groups.begin(), jump_groups.end(), [=](auto i){
                        return std::all_of(jump_groups.begin(), jump_groups.end(), [=](auto j){
                                return i == j ? true : disjoint(JumpGroup<T>::mask(i), JumpGroup<T>::mask(j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(jump_groups.begin(), jump_groups.end(), set_t<T>{}, [](auto result, auto i){
                        return result ^ JumpGroup<T>::mask(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
