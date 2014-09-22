#pragma once
#include <dctl/xstd/limits.hpp>                  // digits
#include <dctl/xstd/masks.hpp>                   // all, any, none
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END

namespace xstd {

BOOST_AUTO_TEST_SUITE(BitIntrinsic)

// the gcc intrinsics are defined for the following types
using UnsignedIntegerTypes = boost::mpl::vector
<
        unsigned,
        unsigned long,
        unsigned long long
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(CountTrailingZeros, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::ctz(masks::none<T>), digits<T>);
        BOOST_CHECK_EQUAL(lib::ctz(masks::all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::ctz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountTrailingZerosNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::ctznz(masks::all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::ctznz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(BitScanForwardNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::bsfnz(masks::all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::bsfnz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLeadingZeros, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::clz(masks::none<T>), digits<T>);
        BOOST_CHECK_EQUAL(lib::clz(masks::all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::clz(b), digits<T> - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLeadingZerosNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::clznz(masks::all<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::clznz(b), digits<T> - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(BitScanReverseNonZero, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::bsrnz(masks::all<T>), digits<T> - 1);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::bsrnz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Popcount, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::popcount(masks::none<T>), 0);

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = masks::one<T> << i;
                BOOST_CHECK_EQUAL(lib::popcount(b), 1);
        }

        for (auto i = 0; i < digits<T>; ++i) {
                for (auto j = i + 1; j < digits<T>; ++j) {
                        auto const b = (masks::one<T> << i) ^ (masks::one<T> << j);
                        BOOST_CHECK_EQUAL(lib::popcount(b), 2);
                }
        }

        for (auto i = 0; i < digits<T>; ++i) {
                auto const b = ~((masks::all<T> >> i) << i);
                BOOST_CHECK_EQUAL(lib::popcount(b), i);
        }

        BOOST_CHECK_EQUAL(lib::popcount(masks::all<T>), digits<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace xstd
