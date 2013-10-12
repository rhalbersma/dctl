#include <algorithm>                            // max_element, is_sorted, adjacent_find
#include <cstdint>                              // uint64_t
#include <functional>                           // greater
#include <initializer_list>                     // initializer_list
#include <iterator>                             // begin, end, cbegin, cend, distance
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit/bit_set.hpp>                 // bit_set

namespace dctl {
namespace bit {

using U = int;

using BitSetTypes = boost::mpl::vector
<
        bit_set<U, uint64_t, 1>,
        bit_set<U, uint64_t, 2>,
        bit_set<U, uint64_t, 3>,
        bit_set<U, uint64_t, 4>
>;

BOOST_AUTO_TEST_SUITE(BitSet)

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultConstructorZeroInitializes, T, BitSetTypes)
{
        /* constexpr */ T b;

        BOOST_CHECK(b.empty());
        BOOST_CHECK_EQUAL(b.size(), std::distance(begin(b), end(b)));
        BOOST_CHECK_EQUAL(b.size(), std::distance(b.rbegin(), b.rend()));
        BOOST_CHECK(begin(b) == end(b));
        BOOST_CHECK(cbegin(b) == cend(b));
        BOOST_CHECK(b.rbegin() == b.rend());
        BOOST_CHECK(b.crbegin() == b.crend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IteratorPairConstructorListInitializes, T, BitSetTypes)
{
        constexpr U a[] = { 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };
        /* constexpr */ auto b = T{std::begin(a), std::end(a)};

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(a), std::end(a), begin(b), end(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InitializerListConstructorListInitializes, T, BitSetTypes)
{
        constexpr U a[]  =  { 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };
        constexpr auto b = T{ 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(a), std::end(a), begin(b), end(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InitializerListAssignmentOperatorListAssigns, T, BitSetTypes)
{
        constexpr U a[] = { 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };
        T b;          b = { 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(a), std::end(a), begin(b), end(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IteratorsTraversenRange, T, BitSetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK_EQUAL(b.empty(), b.size() == 0);
        BOOST_CHECK_EQUAL(std::distance(begin(b), end(b)), b.size());
        BOOST_CHECK_EQUAL(std::distance(b.rbegin(), b.rend()), b.size());

        {
                auto it = begin(b);
                for (; it != end(b); ++it){}
                BOOST_CHECK(it == b.rbegin().base());
        }

        {
                auto it = end(b);
                for (; it != begin(b); --it){}
                BOOST_CHECK(it == b.rend().base());
        }

        {
                auto it = b.rbegin();
                for (; it != b.rend(); ++it){}
                BOOST_CHECK(it.base() == begin(b));
        }

        {
                auto it = b.rend();
                for (; it != b.rbegin(); --it){}
                BOOST_CHECK(it.base() == end(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsBoundedSortedUnique, T, BitSetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK_LT(*std::max_element(begin(b), end(b)), b.max_size());
        BOOST_CHECK(std::is_sorted(begin(b), end(b)));
        BOOST_CHECK(std::is_sorted(b.rbegin(), b.rend(), std::greater<U>()));
        BOOST_CHECK(std::adjacent_find(begin(b), end(b)) == end(b));
}

/*
BOOST_AUTO_TEST_CASE(Members)
{
        //const bitset z { 17, 31, 61 }; // removing const will enable dynamic re-checking of mask
        //std::copy(z.begin(), z.end(), std::ostream_iterator<std::size_t>(std::cout, ",")); std::cout << "\n";

        T a[] = { 17, 31, 61 };
        bitset64 aa;
        aa.insert(std::begin(a), std::end(a));
        bitset64 bb(std::begin(a), std::end(a));
        bitset64 cc(aa);
        BOOST_CHECK(aa == bb);
        BOOST_CHECK(aa == cc);

        std::copy(std::begin(a), std::end(a), std::insert_iterator<bitset64>(aa, aa.end()));

        // initialize-list
        bitset64 x {17, 31, 63};
        std::copy(begin(x), end(x), std::ostream_iterator<int>(std::cout, ",")); std::cout << "\n";

        BOOST_CHECK(std::is_sorted(begin(x), end(x)));
        BOOST_CHECK(std::is_sorted(x.crbegin(), x.crend(), std::greater<int>()));
        BOOST_CHECK(std::adjacent_find(begin(x), end(x)) == end(x));

        T tr[] = { 0, 1, 63, 64, 68, 127 };
        bitset128 b2;
        std::copy(std::begin(tr), std::end(tr), std::insert_iterator<bitset128>(b2, b2.end()));
        std::copy(b2.rbegin(), b2.rend(), std::ostream_iterator<int>(std::cout, ",")); std::cout << "\n";

        BOOST_CHECK((x.find(17) != end(x)) == x.count(17));
        BOOST_CHECK((x.find(18) != end(x)) == x.count(18));

        auto y = x;

        x.insert(12);
        x.erase(17);
        y.insert({1,6});

        // plain for-loop with iterators
        //for (auto it = x.begin(); it != x.end(); ++it) std::cout << *it << ","; std::cout << "\n";
        //for (auto it = y.begin(); it != y.end(); ++it) std::cout << *it << ","; std::cout << "\n";

        // range for-loop with auto (NOTE: can't use auto& or auto const& here)
        //auto i = set_intersection(x, y);
        //for (auto e: i) std::cout << e << ","; std::cout << "\n";

        // STL algorithm iteration
        //auto u = set_union(x,y);
        //std::for_each(u.begin(), u.end(), [](bit_reference<uint64_t> const& elem){ std::cout << elem << ","; }); std::cout << "\n";

        // copy to std::cout
        //auto s = set_symmetric_difference(x, y);
        //std::copy(s.begin(), s.end(), std::ostream_iterator<std::size_t>(std::cout, ",")); std::cout << "\n";

        //auto d1 = set_difference(x, y);
        //auto d2 = set_difference(y, x);
        //for (auto it = d1.begin(); it != d1.end(); ++it) std::cout << *it << ","; std::cout << "\n";
        //for (auto it = d2.begin(); it != d2.end(); ++it) std::cout << *it << ","; std::cout << "\n";

        // x and y don't include each other
        BOOST_CHECK(!set_includes(y, x));
        BOOST_CHECK(!set_includes(x, y));

        // plain for-loop summation equals accumulate algorithm
        auto sum = 0; for (auto it = begin(x); it != end(x); ++it) sum += *it;
        BOOST_CHECK(sum == std::accumulate(begin(x), end(x), 0));
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
