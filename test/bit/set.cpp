#include <algorithm>
#include <iostream>
#include <ios>
#include <iterator>
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit/set.hpp>

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitSet)

BOOST_AUTO_TEST_CASE(Members)
{
        const bitset z { 17, 31, 61 }; // removing const will enable dynamic re-checking of mask
        std::copy(z.begin(), z.end(), std::ostream_iterator<std::size_t>(std::cout, ",")); std::cout << "\n";

        uint64_t a[] = { 17, 31, 61 };
        bitset aa;
        aa.insert(std::begin(a), std::end(a));

        std::copy(std::begin(a), std::end(a), std::insert_iterator<bitset>(aa, aa.end()));
        std::copy(aa.begin(), aa.end(), std::ostream_iterator<std::size_t>(std::cout, ",")); std::cout << "\n";

        // initialize-list
        bitset x {17, 31, 61};

        BOOST_CHECK((x.find(17) != x.end()) == x.count(17));
        BOOST_CHECK((x.find(18) != x.end()) == x.count(18));

        auto y = x;

        x.insert(12);
        x.erase(17);
        y.insert({1,6});

        // plain for-loop using iterators
        for (auto it = x.begin(); it != x.end(); ++it) std::cout << *it << ","; std::cout << "\n";
        for (auto it = y.begin(); it != y.end(); ++it) std::cout << *it << ","; std::cout << "\n";

        // range for-loop using auto (NOTE: can't use auto& or auto const& here)
        auto i = set_intersection(x, y);
        for (auto e: i) std::cout << e << ","; std::cout << "\n";

        // STL algorithm iteration
        auto u = set_union(x,y);
        std::for_each(u.begin(), u.end(), [](bit_reference<uint64_t> const& elem){ std::cout << elem << ","; }); std::cout << "\n";

        // copy to std::cout
        auto s = set_symmetric_difference(x, y);
        std::copy(s.begin(), s.end(), std::ostream_iterator<std::size_t>(std::cout, ",")); std::cout << "\n";

        auto d1 = set_difference(x, y);
        auto d2 = set_difference(y, x);
        for (auto it = d1.begin(); it != d1.end(); ++it) std::cout << *it << ","; std::cout << "\n";
        for (auto it = d2.begin(); it != d2.end(); ++it) std::cout << *it << ","; std::cout << "\n";

        // x and y don't include each other
        BOOST_CHECK(!includes(y, x));
        BOOST_CHECK(!includes(x, y));

        // both include their intersection
        BOOST_CHECK(includes(x, i));
        BOOST_CHECK(includes(y, i));

        // their union includes both
        BOOST_CHECK(includes(u, x));
        BOOST_CHECK(includes(u, y));

        // plain for-loop summation equals accumulate algorithm
        auto sum = 0; for (auto it = x.begin(); it != x.end(); ++it) sum += *it;
        BOOST_CHECK(sum == std::accumulate(x.begin(), x.end(), 0));

        // distance between begin / end equals number of elements
        BOOST_CHECK(std::distance(x.begin(), x.end()) == static_cast<std::ptrdiff_t>(x.size()));

        // sets are sorted containers of unique elements
        BOOST_CHECK(std::is_sorted(x.begin(), x.end()));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
