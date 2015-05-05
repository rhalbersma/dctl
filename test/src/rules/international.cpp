#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, equal_to, less
#include <dctl/rules.hpp>               // International
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                    // adjacent_find, is_sorted
#include <cstddef>                      // size_t
#include <type_traits>                  // is_same
#include <vector>                       // vector

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesInternational)

using T = International;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, long_ranged_tag>::value, "");
        static_assert(!precedence::is_trivial_v<T>, "");

        struct Move
        {
                std::size_t num_captured_;
                constexpr auto num_captured() const noexcept { return num_captured_; }
        };

        auto const moves = std::vector<Move>
        {
                { 1 },
                { 2 },
                { 3 }
        };

        BOOST_CHECK(precedence::is_consistent<T>(begin(moves), end(moves)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
