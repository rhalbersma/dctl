#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/bit/bit.hpp>
#include <dctl/bit/loop.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(TestLookup)

typedef boost::mpl::vector
<
        uint32_t,
        uint64_t
> Unsignedinteger_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(LoopCount, T, Unsignedinteger_types)
{
        BOOST_CHECK_EQUAL(0, loop::count(T(0)));

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK_EQUAL(1, loop::count(b));
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        auto const b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, loop::count(b));
                        else
                                BOOST_CHECK_EQUAL(2, loop::count(b));
                }
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(i, loop::count(b));
        }

        BOOST_CHECK_EQUAL(num_bits<T>::value, loop::count(T(~0)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
