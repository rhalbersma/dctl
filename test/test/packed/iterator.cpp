#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <algorithm>                            // is_sorted
#include <cstdint>
#include <iostream>
#include <ios>
#include <dctl/packed/iterator.hpp>

namespace dctl {
namespace packed {

typedef uint64_t bitset;

BOOST_AUTO_TEST_SUITE(Iterators)

BOOST_AUTO_TEST_CASE(DeBruijn)
{
    auto b = bitset { 0x022fdd63cc95386dULL };
    //for (auto i: b) std::cout << i << " "; std::cout << "\n";
    BOOST_CHECK(std::is_sorted(begin(b), end(b)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace packed
}       // namespace dctl
