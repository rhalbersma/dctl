#include <cstdint>				// uint32_t, uint64_t
#include <limits>                               // digits
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/bit/bit.hpp>
#include <dctl/bit/intrinsic/loop.hpp>

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitIntrinsicLoop)

using UnsignedIntegerTypes = boost::mpl::vector
<
        uint32_t, uint64_t
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(UncheckedCtz, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(loop::unchecked_ctz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(UncheckedClz, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(loop::unchecked_clz(b), std::numeric_limits<T>::digits - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(PopCount, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(loop::popcount(zero<T>()), 0);

        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(loop::popcount(b), 1);
        }

        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                for (auto j = 0; j < std::numeric_limits<T>::digits; ++j) {
                        auto const b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(loop::popcount(b), 0);
                        else
                                BOOST_CHECK_EQUAL(loop::popcount(b), 2);
                }
        }

        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(loop::popcount(b), i);
        }

        BOOST_CHECK_EQUAL(loop::popcount(universe<T>()), std::numeric_limits<T>::digits);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
