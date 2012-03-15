#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include "../../src/bit/Bit.hpp"
#include "../../src/utility/IntegerTypes.hpp"

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(TestBit)

typedef boost::mpl::list
<
        uint8_t,
        uint16_t,
        uint32_t,
        uint64_t
> UnsignedIntegerTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsZero, T, UnsignedIntegerTypes)
{
        const auto b = T(0);
        BOOST_CHECK( is_zero(b));
        BOOST_CHECK(!is_single(b));
        BOOST_CHECK(!is_double(b));
        BOOST_CHECK(!is_multiple(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSingle, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i);
                BOOST_CHECK(!is_zero(b));
                BOOST_CHECK( is_single(b));
                BOOST_CHECK(!is_double(b));
                BOOST_CHECK(!is_multiple(b));

                const auto r = reverse_singlet<T>(i);
                BOOST_CHECK(!is_zero(r));
                BOOST_CHECK( is_single(r));
                BOOST_CHECK(!is_double(r));
                BOOST_CHECK(!is_multiple(r));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDouble, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        const auto b = singlet<T>(i) ^ singlet<T>(j);
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

BOOST_AUTO_TEST_CASE_TEMPLATE(IsMultiple, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value - i; ++j) {
                        const auto b = (singlet<T>(i) - T(1)) << j;
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
                        }
                }
        }
        BOOST_CHECK(is_multiple(T(~0)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IndexDeBruijn, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i);
                BOOST_CHECK_EQUAL(i, index_DeBruijn(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IndexLookup, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i);
                BOOST_CHECK_EQUAL(i, index_lookup(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountKernighan, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(0, count_loop(T(0)));

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i);
                BOOST_CHECK_EQUAL(1, count_loop(b));
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        const auto b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, count_loop(b));
                        else
                                BOOST_CHECK_EQUAL(2, count_loop(b));
                }
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(i, count_loop(b));
        }

        BOOST_CHECK_EQUAL(num_bits<T>::value, count_loop(T(~0)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLookup, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(0, count_lookup(T(0)));

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i);
                BOOST_CHECK_EQUAL(1, count_lookup(b));
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                for (auto j = 0; j < num_bits<T>::value; ++j) {
                        const auto b = singlet<T>(i) ^ singlet<T>(j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, count_lookup(b));
                        else
                                BOOST_CHECK_EQUAL(2, count_lookup(b));
                }
        }

        for (auto i = 0; i < num_bits<T>::value; ++i) {
                const auto b = singlet<T>(i) - 1;
                BOOST_CHECK_EQUAL(i, count_lookup(b));
        }

        BOOST_CHECK_EQUAL(num_bits<T>::value, count_lookup(T(~0)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
