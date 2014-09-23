#include <algorithm>                            // adjacent_find, is_sorted, minmax_element
#include <cstdint>                              // uint64_t
#include <functional>                           // greater, greater_equal, less, less_equal
#include <initializer_list>                     // initializer_list
#include <iterator>                             // begin, end, cbegin, cend, distance
#include <tuple>                                // tie
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bitset.hpp>                      // bitset
#include <iostream>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>
#include <boost/range/end.hpp>

#include <bitset/bitset/exhaustive.hpp>

namespace xstd {

BOOST_AUTO_TEST_SUITE(BitSet)

using U = int;

using SetTypes = boost::mpl::vector
<
        bitset<  0>,
        bitset< 64>,    // Chess
        bitset< 81>,    // Shogi
        bitset< 90>,    // Xiangqi
        bitset<361>     // Go
>;
/*
BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultConstructorZeroInitializes, T, SetTypes)
{
        T b;

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
        auto b = T(std::begin(a), std::end(a));

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
*/
BOOST_AUTO_TEST_CASE_TEMPLATE(Exhaustive, T, SetTypes)
{
        auto constexpr N = T::size();

        // comparators

        BOOST_CHECK(equal<N>());
        BOOST_CHECK(not_equal<N>());

        // modifiers

        BOOST_CHECK(and_assign<N>());
        BOOST_CHECK(or_assign<N>());
        BOOST_CHECK(xor_assign<N>());
        BOOST_CHECK(minus_assign<N>());
        BOOST_CHECK(shift_left_assign<N>());
        BOOST_CHECK(shift_right_assign<N>());

        // observers

        BOOST_CHECK(count<N>());

        // operators

        BOOST_CHECK(op_and<N>());
        BOOST_CHECK(op_or<N>());
        BOOST_CHECK(op_xor<N>());
        BOOST_CHECK(op_minus<N>());
        BOOST_CHECK(op_shift_left<N>());
        BOOST_CHECK(op_shift_right<N>());
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace xstd
