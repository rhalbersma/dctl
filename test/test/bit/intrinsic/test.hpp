#pragma once
#include <cstdint>                                      // uint32_t, uint64_t
#include <limits>                                       // digits
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                         // vector
#include <dctl/bit/detail/intrinsic/builtin.hpp>        // ctznz, clznz, popcount

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitIntrinsicBuiltin)

using UnsignedIntegerTypes = boost::mpl::vector
<
        uint32_t, uint64_t
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(CountTrailingZeros, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = T{1} << i;
                BOOST_CHECK_EQUAL(lib::ctznz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLeadingZeros, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = T{1} << i;
                BOOST_CHECK_EQUAL(lib::clznz(b), std::numeric_limits<T>::digits - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Popcount, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(lib::popcount(T{}), 0);

        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = T{1} << i;
                BOOST_CHECK_EQUAL(lib::popcount(b), 1);
        }

        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                for (auto j = 0; j < std::numeric_limits<T>::digits; ++j) {
                        auto const b = (T{1} << i) ^ (T{1} << j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(lib::popcount(b), 0);
                        else
                                BOOST_CHECK_EQUAL(lib::popcount(b), 2);
                }
        }

        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = (T{1} << i) - 1;
                BOOST_CHECK_EQUAL(lib::popcount(b), i);
        }

        BOOST_CHECK_EQUAL(lib::popcount(~T{}), std::numeric_limits<T>::digits);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
