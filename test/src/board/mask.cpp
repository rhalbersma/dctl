#include <dctl/board/mask.hpp>                  // Column, JumpGroup, Row, Squares
#include <dctl/board/set_type.hpp>                    // set_type
#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsireti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact1012, Compact120, Board<12, 10>, Canadian, SriLankan, Dumm
#include <xstd/cstddef.hpp>                     // _z
#include <range/v3/all.hpp>                     // all_of, view::iota
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

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
        auto const columns = ranges::view::iota(0_z, T::width());

        BOOST_CHECK(
                ranges::all_of(columns, [=](auto i){
                        return Column<T>::mask(Color::black, i) == Column<T>::mask(Color::white, T::width() - 1 - i);
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
                ranges::accumulate(columns, get_set_type<T>{}, [](auto result, auto i){
                        return result ^ Column<T>::mask(Color::white, i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RowsEquivalencePartitionSquares, T, BoardSequence)
{
        using namespace xstd::support_literals;
        auto const rows = ranges::view::iota(0_z, T::height());

        BOOST_CHECK(
                ranges::all_of(rows, [=](auto i){
                        return Row<T>::mask(Color::black, i) == Row<T>::mask(Color::white, T::height() - 1 - i);
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
                ranges::accumulate(rows, get_set_type<T>{}, [](auto result, auto i){
                        return result ^ Row<T>::mask(Color::white, i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpGroupsEquivalencePartitionSquares, T, BoardSequence)
{
        using namespace xstd::support_literals;
        auto const jump_groups = ranges::view::iota(0_z, 4_z);

        BOOST_CHECK(
                ranges::all_of(jump_groups, [=](auto i){
                        return ranges::all_of(jump_groups, [=](auto j){
                                return i == j ? true : disjoint(JumpGroup<T>::mask(i), JumpGroup<T>::mask(j));
                        });
                })
        );

        BOOST_CHECK(
                ranges::accumulate(jump_groups, get_set_type<T>{}, [](auto result, auto i){
                        return result ^ JumpGroup<T>::mask(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
