#include <cstdint>				// uint8_t, uint16_t, uint32_t, uint64_t
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/bit/bit.hpp>
#include <dctl/bit/loop.hpp>
#include <dctl/utility/int.hpp>                 // num_bits

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitLoop)

using UnsignedIntegerTypes = boost::mpl::vector
<
        uint32_t, uint64_t
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Front, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(i, loop::front(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Back, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(i, loop::back(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Size, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(0, loop::size(zero<T>()));

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(1, loop::size(b));
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        auto const b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, loop::size(b));
                        else
                                BOOST_CHECK_EQUAL(2, loop::size(b));
                }
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(i, loop::size(b));
        }

        BOOST_CHECK_EQUAL(num_bits<T>::value, loop::size(universe<T>()));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
