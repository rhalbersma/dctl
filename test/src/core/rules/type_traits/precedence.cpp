#include <dctl/core/rules/type_traits.hpp>                 // is_trivial, is_ordering, is_absolute_modality
#include <dctl/core/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(IsTrivialPrecedence)

        using VariantsTrue = boost::mpl::vector
        <
                checkers, pool, russian, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_trivial_precedence_v<T>);
        }

        using VariantsFalse = boost::mpl::vector
        <
                czech, frisian, international, italian, spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_trivial_precedence_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsQuantityPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, pool, russian, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_quantity_precedence_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                frisian, international, italian, spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_quantity_precedence_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsContentsPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, international, pool, russian, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_contents_precedence_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                frisian, italian, spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_contents_precedence_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsModalityPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, international, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_modality_precedence_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                czech, frisian, italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_modality_precedence_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsOrderingPrecedence)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, frisian, international, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_ordering_precedence_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_ordering_precedence_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PrecedenceTrivialIsConsistentWithTraits)

        using VariantsTrue = boost::mpl::vector
        <
                checkers, czech, frisian, international, italian, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert((
                         is_quantity_precedence_v<T> ||
                         is_contents_precedence_v<T>  ||
                         is_modality_precedence_v<T> ||
                         is_ordering_precedence_v<T> ) ==
                        !is_trivial_precedence_v<T>
                );
        }

BOOST_AUTO_TEST_SUITE_END()
