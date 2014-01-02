#include <cstdint>                              // uint64_t
#include <limits>                               // digits
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/set.hpp>

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(BitBit)

using U = int;

using SetTypes = boost::mpl::vector
<
        Set<U, uint64_t, 1>,
        Set<U, uint64_t, 2>,
        Set<U, uint64_t, 3>,
        Set<U, uint64_t, 4>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsEmpty, T, SetTypes)
{
        auto const b = T{};
        BOOST_CHECK(b.empty());
        BOOST_CHECK(!is_single(b));
        BOOST_CHECK(!is_double(b));
        BOOST_CHECK(!is_multiple(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSingle, T, SetTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                auto const b = T{i};
                BOOST_CHECK(!b.empty());
                BOOST_CHECK( is_single(b));
                BOOST_CHECK(!is_double(b));
                BOOST_CHECK(!is_multiple(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDouble, T, SetTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                for (auto j = 0; j < std::numeric_limits<T>::digits; ++j) {
                        auto const b = T{i, j};
                        if (i == j) {
                                BOOST_CHECK(b.empty());
                        } else {
                                BOOST_CHECK(!b.empty());
                                BOOST_CHECK(!is_single(b));
                                BOOST_CHECK( is_double(b));
                                BOOST_CHECK( is_multiple(b));
                        }
                }
        }
}
/*
BOOST_AUTO_TEST_CASE_TEMPLATE(IsMultiple, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i) {
                for (auto j = 0; j < std::numeric_limits<T>::digits - i; ++j) {
                        auto const b = (singlet<T>(i) - T(1)) << j;
                        switch (i) {
                        case 0:
                                BOOST_CHECK(empty(b));
                                break;
                        case 1:
                                BOOST_CHECK(is_single(b));
                                break;
                        case 2:
                                BOOST_CHECK(is_double(b));
                                BOOST_CHECK(is_multiple(b));
                                break;
                        default:
                                BOOST_CHECK(!empty(b));
                                BOOST_CHECK(!is_single(b));
                                BOOST_CHECK(!is_double(b));
                                BOOST_CHECK( is_multiple(b));
                                break;
                        }
                }
        }
        BOOST_CHECK(is_multiple(T(~0)));
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
