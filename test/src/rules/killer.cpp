#include <dctl/rule_traits.hpp>                 // is_halt_behind_king
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Killer, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_base_of

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesKiller)

using Variants = boost::mpl::vector
<
        Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(RuleTraits, T, Variants)
{
        static_assert(std::is_base_of<T, Killer<T>>{});
        static_assert(is_halt_behind_king_or_v<Killer<T>>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
