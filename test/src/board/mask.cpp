#include <dctl/board/mask/column.hpp>           // column
#include <dctl/board/mask/jump_group.hpp>       // jump_group
#include <dctl/board/mask/row.hpp>              // row
#include <dctl/board/mask/squares.hpp>          // squares
#include <dctl/utility/type_traits.hpp>         // set_t
#include <board/sequence.hpp>                   // micro, mini, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/range/numeric.hpp>              // accumulate
#include <boost/range/irange.hpp>               // irange
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <vector>

namespace dctl {
namespace board {
namespace mask {

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(SquaresSizeEqualsBoardSize, T, board::BoardSequence)
{
        BOOST_CHECK_EQUAL(squares_v<T>.count(), T::size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(columnsEquivalencePartitionSquares, T, board::BoardSequence)
{
        auto const columns = boost::irange(0, T::width);

        BOOST_CHECK(
                boost::algorithm::all_of(columns, [=](auto i){
                        return column<T>{}(color::black, i) == column<T>{}(color::white, T::width - 1 - i);
                })
        );

        BOOST_CHECK(
                boost::algorithm::all_of(columns, [=](auto i){
                        return boost::algorithm::all_of(columns, [=](auto j){
                                return i == j ? true : disjoint(column<T>{}(color::white, i), column<T>{}(color::white, j));
                        });
                })
        );

        BOOST_CHECK(
                boost::accumulate(columns, set_t<T>{}, [](auto result, auto i){
                        return result ^ column<T>{}(color::white, i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(rowsEquivalencePartitionSquares, T, board::BoardSequence)
{
        auto const rows = boost::irange(0, T::height);

        BOOST_CHECK(
                boost::algorithm::all_of(rows, [=](auto i){
                        return row<T, black_>{}(i) == row<T, white_>{}(T::height - 1 - i);
                })
        );

        BOOST_CHECK(
                boost::algorithm::all_of(rows, [=](auto i){
                        return boost::algorithm::all_of(rows, [=](auto j){
                                return i == j ? true : disjoint(row<T, white_>{}(i), row<T, white_>{}(j));
                        });
                })
        );

        BOOST_CHECK(
                boost::accumulate(rows, set_t<T>{}, [](auto result, auto i){
                        return result ^ row<T, white_>{}(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(jump_groupsEquivalencePartitionSquares, T, board::BoardSequence)
{
        auto const jump_groups = std::vector<int>{ 0, 1, 2, 3 };

        BOOST_CHECK(
                boost::algorithm::all_of(jump_groups, [=](auto i){
                        return boost::algorithm::all_of(jump_groups, [=](auto j){
                                return i == j ? true : disjoint(jump_group<T>{}(i), jump_group<T>{}(j));
                        });
                })
        );

        BOOST_CHECK(
                boost::accumulate(jump_groups, set_t<T>{}, [](auto result, auto i){
                        return result ^ jump_group<T>{}(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace mask
}       // namespace board
}       // namespace dctl
