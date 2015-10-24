#include <dctl/rule_traits.hpp>                 // is_complete, is_trivial, is_ordering, is_absolute_modality
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(PrecedenceDetectCompletenessAndTriviality)

        struct NeitherEqualToNorLess {                               };
        struct BothEqualToAndLess    { struct equal_to; struct less; };
        struct EqualToWithoutLess    { struct equal_to;              };
        struct LessWithoutEqualTo    { struct less;                  };

        using VariantsComplete = boost::mpl::vector
        <
                NeitherEqualToNorLess, BothEqualToAndLess
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsComplete, T, VariantsComplete)
        {
                static_assert(precedence::is_complete_v<T>);
        }

        using VariantsIncomplete = boost::mpl::vector
        <
                EqualToWithoutLess, LessWithoutEqualTo
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsIncomplete, T, VariantsIncomplete)
        {
                static_assert(!precedence::is_complete_v<T>);
        }

        using VariantsTrivial = boost::mpl::vector
        <
                NeitherEqualToNorLess
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrivial, T, VariantsTrivial)
        {
                static_assert(precedence::is_trivial_v<T>);
        }

        using VariantsNonTrivial = boost::mpl::vector
        <
                BothEqualToAndLess, EqualToWithoutLess, LessWithoutEqualTo
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsNonTrivial, T, VariantsNonTrivial)
        {
                static_assert(!precedence::is_trivial_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsComplete)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_complete_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsTrivial)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Pool, Russian, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_trivial_v<T>);
        }

        using VariantsFalse = boost::mpl::vector
        <
                Czech, Frisian, International, Italian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_trivial_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsQuantity)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Pool, Russian, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_quantity_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Frisian, International, Italian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_quantity_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsQuality)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, International, Pool, Russian, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_quality_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Frisian, Italian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_quality_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsModality)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_modality_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Czech, Frisian, Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_modality_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceIsOrdering)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!precedence::is_ordering_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(precedence::is_ordering_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceTrivialIsConsistentWithTraits)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert((
                         precedence::is_quantity_v<T> ||
                         precedence::is_quality_v<T>  ||
                         precedence::is_modality_v<T> ||
                         precedence::is_ordering_v<T> ) ==
                        !precedence::is_trivial_v<T>, ""
                );
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
