#include <algorithm>
#include <iostream>
#include <iterator>
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit/set.hpp>

namespace dctl {
namespace bit {

typedef bit_set<uint64_t, 1> bitset;

BOOST_AUTO_TEST_SUITE(BitSet)

BOOST_AUTO_TEST_CASE(Members)
{
        const bitset z { 17, 31, 61 }; // removing const will enable dynamic re-checking of mask
        std::copy(z.begin(), z.end(), std::ostream_iterator<std::size_t>(std::cout, ",")); std::cout << "\n";

        std::size_t a[] = { 17, 31, 61 };
        bitset aa;
        aa.insert(std::begin(a), std::end(a));
        bitset bb(std::begin(a), std::end(a));
        bitset cc(aa);
        BOOST_CHECK(aa == bb);
        BOOST_CHECK(aa == cc);

        std::copy(std::begin(a), std::end(a), std::insert_iterator<bitset>(aa, aa.end()));

        // initialize-list
        bitset x {17, 31, 61};

        BOOST_CHECK((x.find(17) != x.end()) == x.count(17));
        BOOST_CHECK((x.find(18) != x.end()) == x.count(18));

        auto y = x;

        x.insert(12);
        x.erase(17);
        y.insert({1,6});

        // plain for-loop with iterators
        for (auto it = x.begin(); it != x.end(); ++it) std::cout << *it << ","; std::cout << "\n";
        for (auto it = y.begin(); it != y.end(); ++it) std::cout << *it << ","; std::cout << "\n";

        // range for-loop with auto (NOTE: can't use auto& or auto const& here)
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

        // plain for-loop summation equals accumulate algorithm
        auto sum = 0; for (auto it = x.begin(); it != x.end(); ++it) sum += *it;
        BOOST_CHECK(sum == std::accumulate(x.begin(), x.end(), 0));
}

BOOST_AUTO_TEST_CASE(BitSetIntersection)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const i = set_intersection(a, b);

        BOOST_CHECK(includes(a, i));
        BOOST_CHECK(includes(b, i));
        BOOST_CHECK(i == (a & b));
}

BOOST_AUTO_TEST_CASE(BitSetUnion)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const u = set_union(a, b);

        BOOST_CHECK(includes(u, a));
        BOOST_CHECK(includes(u, b));
        BOOST_CHECK(u == (a | b));
}

BOOST_AUTO_TEST_CASE(BitSetSymmetricDifference)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const s = set_symmetric_difference(a, b);
        auto const d_ab = set_difference(a, b);
        auto const d_ba = set_difference(b, a);
        auto const u = set_union(d_ab, d_ba);

        BOOST_CHECK(s == u);
        BOOST_CHECK(s == (a ^ b));
}

BOOST_AUTO_TEST_CASE(BitSetDifference)
{
        bitset const a { 0, 1, 2, 3 };
        bitset const b { 0, 1, 4, 5 };

        auto const d_ab = set_difference(a, b);
        auto const d_ba = set_difference(b, a);

        BOOST_CHECK( includes(a, d_ab));
        BOOST_CHECK(!includes(a, d_ba));
        BOOST_CHECK( includes(b, d_ba));
        BOOST_CHECK(!includes(b, d_ab));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
