#include <cstdint>				// uint8_t, uint16_t, uint32_t, uint64_t
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/bit/bit.hpp>
#include <dctl/bit/lookup.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(TestLookup)

typedef boost::mpl::vector
<
        //uint8_t,
	//uint16_t,
        //uint32_t,
        uint64_t
> UnsignedIntegerTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(LookupCount, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(0, lookup::count(zero<T>()));

        for (std::size_t i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(1, lookup::count(b));
        }

        for (std::size_t i = 0; i < num_bits<T>::value; ++i) {
                for (std::size_t j = 0; j < num_bits<T>::value; ++j) {
                        auto const b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, lookup::count(b));
                        else
                                BOOST_CHECK_EQUAL(2, lookup::count(b));
                }
        }

        for (std::size_t i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(i, lookup::count(b));
        }

        BOOST_CHECK_EQUAL(num_bits<T>::value, lookup::count(universe<T>()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(LookupIndex, T, UnsignedIntegerTypes)
{
        for (std::size_t i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(i, lookup::index(b));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
