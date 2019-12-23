//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/sequence.hpp>              // nano, micro, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, basic_board<12, 10>, canadian, srilankan, dumm
#include <core/board/transform.hpp>             // is_involution, is_idempotent
#include <dctl/core/board/basic_board.hpp>      // board
#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/rules/type_traits.hpp>      // is_empty, is_pushable, is_jumpable, invert
#include <dctl/core/state/color.hpp>
#include <boost/range/irange.hpp>               // irange
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_CASE_TEMPLATE
#include <algorithm>                            // all_of
#include <numeric>                              // accumulate
#include <type_traits>                          // is_same

BOOST_AUTO_TEST_SUITE(BoardTraits)

using namespace dctl::core;

BOOST_AUTO_TEST_CASE(IsEmpty)
{
        static_assert( is_placeable_v<basic_board<rectangular<1, 1>>>);
        static_assert( is_placeable_v<basic_board<rectangular<2, 1, 0>>>);
        static_assert( is_placeable_v<basic_board<rectangular<1, 2, 0>>>);
}

BOOST_AUTO_TEST_CASE(IsPushable)
{
        static_assert(!is_pushable_v<basic_board<rectangular<1, 1>>>);
        static_assert(!is_pushable_v<basic_board<rectangular<2, 1>>>);
        static_assert(!is_pushable_v<basic_board<rectangular<1, 2>>>);
        static_assert(    is_pushable_v<basic_board<rectangular<2, 2>>>);
}

BOOST_AUTO_TEST_CASE(IsJumpable)
{
        static_assert(!is_jumpable_v<basic_board<rectangular<2, 2>>>);
        static_assert(!is_jumpable_v<basic_board<rectangular<3, 2>>>);
        static_assert(!is_jumpable_v<basic_board<rectangular<2, 3>>>);
        static_assert(!is_jumpable_v<basic_board<rectangular<3, 3, 0>>>);
        static_assert(    is_jumpable_v<basic_board<rectangular<3, 3>>>);
        static_assert(    is_jumpable_v<basic_board<rectangular<4, 3, 0>>>);
        static_assert(    is_jumpable_v<basic_board<rectangular<3, 4, 0>>>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRegular, T, BoardSequence)
{
        static_assert(is_placeable_v<T>);
        static_assert(is_pushable_v<T>);
        static_assert(is_jumpable_v<T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SquaresCountEqualsBoardSize, T, BoardSequence)
{
        using mask_type = basic_mask<T>;
        BOOST_CHECK_EQUAL(mask_type::squares.ssize(), T::size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ColumnsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const files = boost::irange(0, T::width);
        using mask_type = basic_mask<T>;

        BOOST_CHECK(
                std::all_of(files.begin(), files.end(), [=](auto i) {
                        return mask_type::file(black_c, i) == mask_type::file(white_c, T::width - 1 - i);
                })
        );

        BOOST_CHECK(
                std::all_of(files.begin(), files.end(), [=](auto i) {
                        return std::all_of(files.begin(), files.end(), [=](auto j) {
                                return i == j ? true : disjoint(mask_type::file(white_c, i), mask_type::file(white_c, j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(files.begin(), files.end(), set_t<mask_type>{}, [](auto result, auto i) {
                        return result ^ mask_type::file(white_c, i);
                }) == mask_type::squares
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RowsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const rows = boost::irange(0, T::height);
        using mask_type = basic_mask<T>;

        BOOST_CHECK(
                std::all_of(rows.begin(), rows.end(), [=](auto i) {
                        return mask_type::rank(black_c, i) == mask_type::rank(white_c, T::height - 1 - i);
                })
        );

        BOOST_CHECK(
                std::all_of(rows.begin(), rows.end(), [=](auto i) {
                        return std::all_of(rows.begin(), rows.end(), [=](auto j) {
                                return i == j ? true : disjoint(mask_type::rank(white_c, i), mask_type::rank(white_c, j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(rows.begin(), rows.end(), set_t<mask_type>{}, [](auto result, auto i) {
                        return result ^ mask_type::rank(white_c, i);
                }) == mask_type::squares
        );
}

BOOST_AUTO_TEST_SUITE_END()
