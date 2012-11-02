#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/bit/bit.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(TestBit)

typedef boost::mpl::vector
<
        uint32_t,
        uint64_t
> Unsignedinteger_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsZero, T, Unsignedinteger_types)
{
        auto const b = T(0);
        BOOST_CHECK( is_zero(b));
        BOOST_CHECK(!is_single(b));
        BOOST_CHECK(!is_double(b));
        BOOST_CHECK(!is_multiple(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSingle, T, Unsignedinteger_types)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                auto const b = singlet<T>(i);
                BOOST_CHECK(!is_zero(b));
                BOOST_CHECK( is_single(b));
                BOOST_CHECK(!is_double(b));
                BOOST_CHECK(!is_multiple(b));

                auto const r = reverse_singlet<T>(i);
                BOOST_CHECK(!is_zero(r));
                BOOST_CHECK( is_single(r));
                BOOST_CHECK(!is_double(r));
                BOOST_CHECK(!is_multiple(r));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDouble, T, Unsignedinteger_types)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        auto const b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j) {
                                BOOST_CHECK(is_zero(b));
                        } else {
                                BOOST_CHECK(!is_zero(b));
                                BOOST_CHECK(!is_single(b));
                                BOOST_CHECK( is_double(b));
                                BOOST_CHECK( is_multiple(b));
                        }
                }
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsMultiple, T, Unsignedinteger_types)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value - i; ++j) {
                        auto const b = (singlet<T>(i) - T(1)) << j;
                        switch (i) {
                        case 0:
                                BOOST_CHECK(is_zero(b));
                                break;
                        case 1:
                                BOOST_CHECK(is_single(b));
                                break;
                        case 2:
                                BOOST_CHECK(is_double(b));
                                BOOST_CHECK(is_multiple(b));
                                break;
                        default:
                                BOOST_CHECK(!is_zero(b));
                                BOOST_CHECK(!is_single(b));
                                BOOST_CHECK(!is_double(b));
                                BOOST_CHECK( is_multiple(b));
                                break;
                        }
                }
        }
        BOOST_CHECK(is_multiple(T(~0)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl