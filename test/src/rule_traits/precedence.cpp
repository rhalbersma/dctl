#include <dctl/rule_traits.hpp>                 // is_complete, is_trivial, is_king_order, is_absolute_with_king
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

struct NeitherEqualToNorLess {                               };
struct BothEqualToAndLess    { struct equal_to; struct less; };
struct EqualToWithoutLess    { struct equal_to;              };
struct LessWithoutEqualTo    { struct less;                  };

BOOST_AUTO_TEST_SUITE(PrecedenceIsComplete)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai, NeitherEqualToNorLess, BothEqualToAndLess
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_complete_v<T>, "");
        }

        using VariantsFalse = boost::mpl::vector
        <
                EqualToWithoutLess, LessWithoutEqualTo
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_complete_v<T>, "");
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsTrivial)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Pool, Russian, Thai, NeitherEqualToNorLess
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_trivial_v<T>, "");
        }

        using VariantsFalse = boost::mpl::vector
        <
                Czech, Frisian, International, Italian, Spanish, BothEqualToAndLess, EqualToWithoutLess, LessWithoutEqualTo
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_trivial_v<T>, "");
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsKingOrder)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_king_order_v<T>, "");
        }

        using VariantsTrue = boost::mpl::vector
        <
                Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_king_order_v<T>, "");
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsAbsoluteWithKing)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Frisian, International, Italian, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_absolute_with_king_v<T>, "");
        }

        using VariantsTrue = boost::mpl::vector
        <
                Czech
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_absolute_with_king_v<T>, "");
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
