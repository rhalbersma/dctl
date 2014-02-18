#include <cstdint>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit/set.hpp>                     // Set

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/end.hpp>
#include <dctl/egdb/index.hpp>
#include <iostream>

namespace dctl {
namespace egdb {

BOOST_AUTO_TEST_SUITE(EndgameDatabaseIndex)

using U = int;

using SetTypes = boost::mpl::vector
<
        bit::Set<U, uint64_t, 1>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(ColexSubsetRank, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2 };
        constexpr auto e = T{ T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK_EQUAL(colex_subset_rank(b), 0);
        BOOST_CHECK_EQUAL(colex_subset_rank(e), binomial(T{}.max_size(), e.size()) - 1);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
