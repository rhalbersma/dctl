#include <dctl/rules/killer.hpp>        // Killer
#include <dctl/rule_traits.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace killer {

BOOST_AUTO_TEST_SUITE(RulesVariantsKiller)

using T = Rules<>;

BOOST_AUTO_TEST_CASE(Traits)
{
        static_assert(is_directly_halt_after_final_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace killer
}       // namespace dctl
