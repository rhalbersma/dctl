#include <dctl/rule_traits.hpp>                 // is_trivial, is_ordering, is_absolute_modality
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(IsTrivialPrecedence)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Pool, Russian, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(!is_nontrivial_precedence_v<T>);
        }

        using VariantsFalse = boost::mpl::vector
        <
                Czech, Frisian, International, Italian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(is_nontrivial_precedence_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsQuantityPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Pool, Russian, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_quantity_precedence_or_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Frisian, International, Italian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_quantity_precedence_or_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsQualityPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, International, Pool, Russian, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_quality_precedence_or_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Frisian, Italian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_quality_precedence_or_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsModalityPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_modality_precedence_or_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Czech, Frisian, Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_modality_precedence_or_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsOrderingPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_ordering_precedence_or_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_ordering_precedence_or_v<T>);
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
                        is_quantity_precedence_or_v<T> ||
                        is_quality_precedence_or_v<T>  ||
                        is_modality_precedence_or_v<T> ||
                        is_ordering_precedence_or_v<T> ) ==
                        is_nontrivial_precedence_v<T>, ""
                );
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
