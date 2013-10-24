#include <algorithm>                            // minmax_element, adjacent_find
#include <cstdint>                              // uint64_t
#include <functional>                           // greater_equal, less_equal
#include <initializer_list>                     // initializer_list
#include <iterator>                             // begin, end, cbegin, cend, distance
#include <tuple>                                // tie
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
        BOOST_CHECK_EQUAL(b.size(), 0);

        BOOST_CHECK(begin(b) == end(b));
        BOOST_CHECK(cbegin(b) == cend(b));
        BOOST_CHECK(b.rbegin() == b.rend());
        BOOST_CHECK(b.crbegin() == b.crend());

        BOOST_CHECK_EQUAL(std::distance(begin(b), end(b)), b.size());
        BOOST_CHECK_EQUAL(std::distance(cbegin(b), cend(b)), b.size());
        BOOST_CHECK_EQUAL(std::distance(b.rbegin(), b.rend()), b.size());
        BOOST_CHECK_EQUAL(std::distance(b.crbegin(), b.crend()), b.size());
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

BOOST_AUTO_TEST_CASE_TEMPLATE(IteratorsTraverseRange, T, BitSetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK(!b.empty());
        BOOST_CHECK_NE(b.size(), 0);

        BOOST_CHECK_EQUAL(std::distance(begin(b), end(b)), b.size());
        BOOST_CHECK_EQUAL(std::distance(cbegin(b), cend(b)), b.size());
        BOOST_CHECK_EQUAL(std::distance(b.rbegin(), b.rend()), b.size());
        BOOST_CHECK_EQUAL(std::distance(b.crbegin(), b.crend()), b.size());

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

BOOST_AUTO_TEST_CASE_TEMPLATE(IsBounded, T, BitSetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };
        decltype(begin(b)) min, max;
        std::tie(min, max) = std::minmax_element(begin(b), end(b));

        BOOST_CHECK(0 <= *begin(b) && *begin(b) == *min);
        BOOST_CHECK(*max == *b.rbegin() && *b.rbegin() < b.max_size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsStrictlyIncreasing, T, BitSetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T{}.max_size() - 2, T{}.max_size() - 1 };

        BOOST_CHECK(std::adjacent_find(begin(b), end(b), std::greater_equal<U>()) == end(b));
        BOOST_CHECK(std::adjacent_find(b.rbegin(), b.rend(), std::less_equal<U>()) == b.rend());
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
