#pragma once
#include <dctl/bit/traits.hpp>                  // all, any, none, digits
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/contains.hpp>               // contains
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <cstddef>                              // size_t
#include <cstdint>                              // uint32_t, uint64_t, uintptr_t

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitIntrinsic)

// the gcc intrinsics are defined for the following types
using UnsignedIntegerTypes = boost::mpl::vector
<
        unsigned,
        unsigned long,
        unsigned long long
>;

// all other unsigned integer types shall map to the above three types
BOOST_MPL_ASSERT((boost::mpl::contains<UnsignedIntegerTypes, uint32_t>));
BOOST_MPL_ASSERT((boost::mpl::contains<UnsignedIntegerTypes, uint64_t>));
BOOST_MPL_ASSERT((boost::mpl::contains<UnsignedIntegerTypes, uintptr_t>));
BOOST_MPL_ASSERT((boost::mpl::contains<UnsignedIntegerTypes, std::size_t>));

BOOST_AUTO_TEST_CASE_TEMPLATE(CountTrailingZeros, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::ctz(none<T>), digits<T>);
        BOOST_CHECK_EQUAL(lib::ctz(all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::ctz(mask), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountTrailingZerosNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::ctznz(all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::ctznz(mask), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(BitScanForwardNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::bsfnz(all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::bsfnz(mask), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLeadingZeros, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::clz(none<T>), digits<T>);
        BOOST_CHECK_EQUAL(lib::clz(all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::clz(mask), digits<T> - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLeadingZerosNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::clznz(all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::clznz(mask), digits<T> - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(BitScanReverseNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::bsrnz(all<T>), digits<T> - 1);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::bsrnz(mask), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Popcount, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::popcount(none<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = one<T> << i;
                BOOST_CHECK_EQUAL(lib::popcount(mask), 1);
        }

        for (auto i = 0; i < digits<T>; ++i) {
                for (auto j = i + 1; j < digits<T>; ++j) {
                        auto const mask = (one<T> << i) ^ (one<T> << j);
                        BOOST_CHECK_EQUAL(lib::popcount(mask), 2);
                }
        }

        for (auto i = 0; i < digits<T>; ++i) {
                auto const mask = ~((all<T> >> i) << i);
                BOOST_CHECK_EQUAL(lib::popcount(mask), i);
        }

        BOOST_CHECK_EQUAL(lib::popcount(all<T>), digits<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
