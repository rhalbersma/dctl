#include <core/board/sequence.hpp>                   // micro, mini, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <core/board/transform.hpp>                  // is_involution, is_idempotent
#include <dctl/core/board/rectangular.hpp>           // rectangular
#include <dctl/core/board_traits.hpp>                // is_empty, is_pushable, is_jumpable, invert, add_orthogonal_captures, remove_orthogonal_captures
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/range/irange.hpp>               // irange
#include <boost/range/numeric.hpp>              // accumulate
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE
#include <type_traits>                          // is_same

BOOST_AUTO_TEST_SUITE(BoardTraits)

using namespace dctl;

BOOST_AUTO_TEST_CASE(IsEmpty)
{
        //static_assert(!is_placeable<rectangular<0, 0>>);
        //static_assert(!is_placeable<rectangular<0, 1>>);
        //static_assert(!is_placeable<rectangular<1, 0>>);
        //static_assert(!is_placeable<board::rectangular<1, 1, true>>);
        static_assert( is_placeable<board::rectangular<1, 1>>);
        static_assert( is_placeable<board::rectangular<2, 1, true>>);
        static_assert( is_placeable<board::rectangular<1, 2, true>>);
}

BOOST_AUTO_TEST_CASE(IsPushable)
{
        static_assert(!is_pushable<board::rectangular<1, 1>>);
        static_assert(!is_pushable<board::rectangular<2, 1>>);
        static_assert(!is_pushable<board::rectangular<1, 2>>);
        static_assert( is_pushable<board::rectangular<2, 2>>);
}

BOOST_AUTO_TEST_CASE(IsJumpable)
{
        static_assert(!is_jumpable<board::rectangular<2, 2>>);
        static_assert(!is_jumpable<board::rectangular<3, 2>>);
        static_assert(!is_jumpable<board::rectangular<2, 3>>);
        static_assert(!is_jumpable<board::rectangular<3, 3, true>>);
        static_assert( is_jumpable<board::rectangular<3, 3>>);
        static_assert( is_jumpable<board::rectangular<4, 3, true>>);
        static_assert( is_jumpable<board::rectangular<3, 4, true>>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRegular, T, board::BoardSequence)
{
        static_assert(is_placeable<T>);
        static_assert(is_pushable<T>);
        static_assert(is_jumpable<T>);
}

BOOST_AUTO_TEST_CASE(Invert)
{
        static_assert(std::is_same<invert_t<board::checkers>, board::italian >{});
        static_assert(std::is_same<invert_t<board::italian >, board::checkers>{});

        static_assert(std::is_same<invert_t<board::canadian >, board::srilankan>{});
        static_assert(std::is_same<invert_t<board::srilankan>, board::canadian >{});
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InvertIsInvolution, T, board::BoardSequence)
{
        static_assert(board::traits::is_involution_v<invert_t, T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AddRemoveOrthogonalCapturesAreIdemPotent, T, board::BoardSequence)
{
        static_assert(board::traits::is_idempotent_v<   add_orthogonal_captures_t, T>);
        static_assert(board::traits::is_idempotent_v<remove_orthogonal_captures_t, T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AddRemoveOrthogonalCapturesCanBeIdentity, T, board::BoardSequence)
{
        static_assert(!T::is_orthogonal_jump || board::traits::is_identity_v<   add_orthogonal_captures_t, T>);
        static_assert( T::is_orthogonal_jump || board::traits::is_identity_v<remove_orthogonal_captures_t, T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SquaresSizeEqualsBoardSize, T, board::BoardSequence)
{
        BOOST_CHECK_EQUAL(squares_v<T>.size(), T::size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColumnsEquivalencePartitionSquares, T, board::BoardSequence)
{
        auto const columns = boost::irange(0, T::width);

        BOOST_CHECK(
                boost::algorithm::all_of(columns, [=](auto i){
                        return column_v<T, black_>(i) == column_v<T, white_>(T::width - 1 - i);
                })
        );

        BOOST_CHECK(
                boost::algorithm::all_of(columns, [=](auto i){
                        return boost::algorithm::all_of(columns, [=](auto j){
                                return i == j ? true : disjoint(column_v<T, white_>(i), column_v<T, white_>(j));
                        });
                })
        );

        BOOST_CHECK(
                boost::accumulate(columns, set_t<T>{}, [](auto result, auto i){
                        return result ^ column_v<T, white_>(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RowsEquivalencePartitionSquares, T, board::BoardSequence)
{
        auto const rows = boost::irange(0, T::height);

        BOOST_CHECK(
                boost::algorithm::all_of(rows, [=](auto i){
                        return row_v<T, black_>(i) == row_v<T, white_>(T::height - 1 - i);
                })
        );

        BOOST_CHECK(
                boost::algorithm::all_of(rows, [=](auto i){
                        return boost::algorithm::all_of(rows, [=](auto j){
                                return i == j ? true : disjoint(row_v<T, white_>(i), row_v<T, white_>(j));
                        });
                })
        );

        BOOST_CHECK(
                boost::accumulate(rows, set_t<T>{}, [](auto result, auto i){
                        return result ^ row_v<T, white_>(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_SUITE_END()
