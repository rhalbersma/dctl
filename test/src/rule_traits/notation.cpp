#include <dctl/rule_traits.hpp>                 // pushsep, jumpsep
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(PushSep)

        using VariantsHyphen = boost::mpl::vector
        <
                checkers, czech, frisian, international, italian, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsHyphen, T, VariantsHyphen)
        {
                static_assert(pushsep_v<T> == '-');
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(JumpSep)

        using VariantsCross = boost::mpl::vector
        <
                checkers, frisian, international, italian, pool, spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsCross, T, VariantsCross)
        {
                static_assert(jumpsep_v<T> == 'x');
        }

        using VariantsColon = boost::mpl::vector
        <
                czech, russian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsColon, T, VariantsColon)
        {
                static_assert(jumpsep_v<T> == ':');
        }

        using VariantsHyphen = boost::mpl::vector
        <
                thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsHyphen, T, VariantsHyphen)
        {
                static_assert(jumpsep_v<T> == '-');
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
