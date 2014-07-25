#include <dctl/rules/variants/killer.hpp>       // Killer
#include <dctl/rules/traits.hpp>
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsKiller)

using T = Killer;

BOOST_AUTO_TEST_CASE(Traits)
{
        static_assert(is_directly_halt_after_final_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
