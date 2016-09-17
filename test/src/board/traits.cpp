#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsiretti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact_10_12, Compact_12_10, Rectangular<12, 10>, Canadian, SriLankan, Dumm
#include <board/transform.hpp>                  // is_involution, is_idempotent
#include <dctl/board/rectangular.hpp>           // rectangular
#include <dctl/board_traits.hpp>                // is_empty, is_pushable, is_jumpable, invert, add_orthogonal_captures, remove_orthogonal_captures
#include <dctl/utility/logic.hpp>               // implies
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE
#include <type_traits>                          // is_same

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(BoardTraits)

BOOST_AUTO_TEST_CASE(IsEmpty)
{
        //static_assert(!is_placeable<Rectangular<0, 0>>);
        //static_assert(!is_placeable<Rectangular<0, 1>>);
        //static_assert(!is_placeable<Rectangular<1, 0>>);
        static_assert(!is_placeable<Rectangular<1, 1, true>>);
        static_assert( is_placeable<Rectangular<1, 1>>);
        static_assert( is_placeable<Rectangular<2, 1, true>>);
        static_assert( is_placeable<Rectangular<1, 2, true>>);
}

BOOST_AUTO_TEST_CASE(IsPushable)
{
        static_assert(!is_pushable<Rectangular<1, 1>>);
        static_assert(!is_pushable<Rectangular<2, 1>>);
        static_assert(!is_pushable<Rectangular<1, 2>>);
        static_assert( is_pushable<Rectangular<2, 2>>);
}

BOOST_AUTO_TEST_CASE(IsJumpable)
{
        static_assert(!is_jumpable<Rectangular<2, 2>>);
        static_assert(!is_jumpable<Rectangular<3, 2>>);
        static_assert(!is_jumpable<Rectangular<2, 3>>);
        static_assert(!is_jumpable<Rectangular<3, 3, true>>);
        static_assert( is_jumpable<Rectangular<3, 3>>);
        static_assert( is_jumpable<Rectangular<4, 3, true>>);
        static_assert( is_jumpable<Rectangular<3, 4, true>>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRegular, T, BoardSequence)
{
        static_assert(is_placeable<T>);
        static_assert(is_pushable<T>);
        static_assert(is_jumpable<T>);
}

BOOST_AUTO_TEST_CASE(Invert)
{
        static_assert(std::is_same<invert_t<Checkers>, xstd::_t<Italian >>{});
        static_assert(std::is_same<invert_t<Italian >, xstd::_t<Checkers>>{});

        static_assert(std::is_same<invert_t<Canadian >, xstd::_t<SriLankan>>{});
        static_assert(std::is_same<invert_t<SriLankan>, xstd::_t<Canadian >>{});
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InvertIsInvolution, T, BoardSequence)
{
        static_assert(traits::is_involution_v<invert_t, T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AddRemoveOrthogonalCapturesAreIdemPotent, T, BoardSequence)
{
        static_assert(traits::is_idempotent_v<   add_orthogonal_captures_t, T>);
        static_assert(traits::is_idempotent_v<remove_orthogonal_captures_t, T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(AddRemoveOrthogonalCapturesCanBeIdentity, T, BoardSequence)
{
        static_assert(util::implies( T::is_orthogonal_captures, traits::is_identity_v<   add_orthogonal_captures_t, T>));
        static_assert(util::implies(!T::is_orthogonal_captures, traits::is_identity_v<remove_orthogonal_captures_t, T>));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
