#include <cstdint>				// uint8_t, uint16_t, uint32_t, uint64_t
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/bit/bit.hpp>
#include <dctl/bit/intrinsic/loop.hpp>
#include <dctl/utility/int.hpp>                 // num_bits

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitLoop)

using UnsignedIntegerTypes = boost::mpl::vector
<
        uint32_t, uint64_t
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Ctz, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(loop::ctz(b), i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Clz, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(loop::clz(b), num_bits<T>::value - 1 - i);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Popcount, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(loop::popcount(zero<T>()), 0);

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(loop::popcount(b), 1);
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        auto const b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(loop::popcount(b), 0);
                        else
                                BOOST_CHECK_EQUAL(loop::popcount(b), 2);
                }
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(loop::popcount(b), i);
        }

        BOOST_CHECK_EQUAL(loop::popcount(universe<T>()), num_bits<T>::value);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
