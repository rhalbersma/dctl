#include <algorithm>                            // adjacent_find, is_sorted, minmax_element
#include <cstdint>                              // uint64_t
#include <functional>                           // greater, greater_equal, less, less_equal
#include <initializer_list>                     // initializer_list
#include <iterator>                             // begin, end, cbegin, cend, distance
#include <tuple>                                // tie
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/xstd/bitset.hpp>                     // Set
#include <iostream>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>
#include <boost/range/end.hpp>

#include <xstd/bitset/exhaustive.hpp>

namespace xstd {

BOOST_AUTO_TEST_SUITE(BitSet)

using U = int;

using SetTypes = boost::mpl::vector
<
        bitset< 64>,    // Chess
        bitset< 81>,    // Shogi
        bitset< 90>,    // Xiangqi
        bitset<361>     // Go
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultConstructorZeroInitializes, T, SetTypes)
{
        /* constexpr */ T b;

        BOOST_CHECK(b.none());
        BOOST_CHECK_EQUAL(b.count(), 0);
        BOOST_CHECK(  begin(b) ==   end(b));
        BOOST_CHECK( cbegin(b) ==  cend(b));
        BOOST_CHECK( rbegin(b) ==  rend(b));
        BOOST_CHECK(crbegin(b) == crend(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IteratorPairConstructorListInitializes, T, SetTypes)
{
        constexpr U a[] = { 0, 1, 2, T::size() - 2, T::size() - 1 };
        /* constexpr */ auto b = T(std::begin(a), std::end(a));

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(a), std::end(a), begin(b), end(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InitializerListConstructorListInitializes, T, SetTypes)
{
        constexpr U a[]  =  { 0, 1, 2, T::size() - 2, T::size() - 1 };
        constexpr auto b = T{ 0, 1, 2, T::size() - 2, T::size() - 1 };

        BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(a), std::end(a), begin(b), end(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(DistanceBeginEndEqualsSize, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::size() - 2, T::size() - 1 };

        BOOST_CHECK_EQUAL(std::distance(  begin(b),   end(b)), b.count());
        BOOST_CHECK_EQUAL(std::distance( cbegin(b),  cend(b)), b.count());
        BOOST_CHECK_EQUAL(std::distance( rbegin(b),  rend(b)), b.count());
        BOOST_CHECK_EQUAL(std::distance(crbegin(b), crend(b)), b.count());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSorted, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::size() - 2, T::size() - 1 };

        BOOST_CHECK(boost::is_sorted(b                            , std::less<U>{})   );
        BOOST_CHECK(boost::is_sorted(b | boost::adaptors::reversed, std::greater<U>{}));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsStrictlyIncreasing, T, SetTypes)
{
        constexpr auto b = T{ 0, 1, 2, T::size() - 2, T::size() - 1 };

        BOOST_CHECK(boost::adjacent_find(b                            , std::greater_equal<U>{}) == boost::end(b) );
        BOOST_CHECK(boost::adjacent_find(b | boost::adaptors::reversed, std::less_equal<U>{})    == boost::rend(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Exhaustive, T, SetTypes)
{
        // modifiers

        BOOST_CHECK(and_assign<T>());
        BOOST_CHECK(or_assign<T>());
        BOOST_CHECK(xor_assign<T>());
        BOOST_CHECK(minus_assign<T>());
        BOOST_CHECK(shift_left_assign<T>());
        BOOST_CHECK(shift_right_assign<T>());

        // observers
        BOOST_CHECK(count<T>());

        // non-members
        BOOST_CHECK(op_and<T>());
        BOOST_CHECK(op_or<T>());
        BOOST_CHECK(op_xor<T>());
        BOOST_CHECK(op_minus<T>());
        BOOST_CHECK(shift_left<T>());
        BOOST_CHECK(shift_right<T>());
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace xstd
