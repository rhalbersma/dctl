#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                            // is_sorted
#include <cstdint>                              // uint64_t
#include <iterator>                             // back_inserter
#include <vector>                               // vector
#include <dctl/packed/raw.hpp>                  // begin, end

namespace dctl {
namespace packed {

BOOST_AUTO_TEST_SUITE(Raw)

BOOST_AUTO_TEST_CASE(DeBruijn)
{
        auto const b = uint64_t { 0x022fdd63cc95386dULL };
        BOOST_CHECK(std::is_sorted(begin(b), end(b)));

        std::vector<int> const v1 = { 0, 2, 3, 5, 6, 11, 12, 13, 16, 18, 20, 23, 26, 27, 30, 31, 32, 33, 37, 38, 40, 42, 43, 44, 46, 47, 48, 49, 50, 51, 53, 57 };
        BOOST_CHECK_EQUAL(size(b), v1.size());

        std::vector<int> v2;
        v2.reserve(v1.size());
        std::copy(begin(b), end(b), std::back_inserter(v2));
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(v1), end(v1), begin(v2), end(v2));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace packed
}       // namespace dctl
