#include <board/sequence.hpp>                   // micro, mini, checkers, roman, spantsireti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, compact_10_12, compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/board/rectangular.hpp>           // rectangular
#include <dctl/board/traits.hpp>                // is_empty, is_pushable, is_jumpable
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(BoardTraits)

BOOST_AUTO_TEST_CASE(IsEmpty)
{
        static_assert(!is_placeable<rectangular<0, 0>>);
        static_assert(!is_placeable<rectangular<0, 1>>);
        static_assert(!is_placeable<rectangular<1, 0>>);
        static_assert(!is_placeable<rectangular<1, 1, true>>);
        static_assert( is_placeable<rectangular<1, 1>>);
        static_assert( is_placeable<rectangular<2, 1, true>>);
        static_assert( is_placeable<rectangular<1, 2, true>>);
}

BOOST_AUTO_TEST_CASE(IsPushable)
{
        static_assert(!is_pushable<rectangular<1, 1>>);
        static_assert(!is_pushable<rectangular<2, 1>>);
        static_assert(!is_pushable<rectangular<1, 2>>);
        static_assert( is_pushable<rectangular<2, 2>>);
}

BOOST_AUTO_TEST_CASE(IsJumpable)
{
        static_assert(!is_jumpable<rectangular<2, 2>>);
        static_assert(!is_jumpable<rectangular<3, 2>>);
        static_assert(!is_jumpable<rectangular<2, 3>>);
        static_assert(!is_jumpable<rectangular<3, 3, true>>);
        static_assert( is_jumpable<rectangular<3, 3>>);
        static_assert( is_jumpable<rectangular<4, 3, true>>);
        static_assert( is_jumpable<rectangular<3, 4, true>>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRegular, T, BoardSequence)
{
        static_assert(is_placeable<T>);
        static_assert(is_pushable<T>);
        static_assert(is_jumpable<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
