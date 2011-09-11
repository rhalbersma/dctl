#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include "../../src/bit/Bit.h"
#include "../../src/utility/IntegerTypes.h"

namespace dctl {
namespace bit {

BOOST_AUTO_TEST_SUITE(TestBit)

typedef boost::mpl::list<uint8_t, uint16_t, uint32_t, uint64_t> UnsignedIntegerTypes;

template<typename T>
struct NUM_BITS
{
        enum { value = 8 * sizeof(T) };
};

BOOST_AUTO_TEST_CASE_TEMPLATE(IsZero, T, UnsignedIntegerTypes)
{
        T b(0);
        BOOST_CHECK_EQUAL(true,  is_zero(b));
        BOOST_CHECK_EQUAL(false, is_single(b));
        BOOST_CHECK_EQUAL(false, is_double(b));
        BOOST_CHECK_EQUAL(false, is_multiple(b));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSingle, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = T(1) << i;
                BOOST_CHECK_EQUAL(false, is_zero(b));
                BOOST_CHECK_EQUAL(true,  is_single(b));
                BOOST_CHECK_EQUAL(false, is_double(b));
                BOOST_CHECK_EQUAL(false, is_multiple(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDouble, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                for (auto j = 0; j < NUM_BITS<T>::value; ++j) {                                
                        T b = (T(1) << i) ^ (T(1) << j);
                        if (i == j) {
                                BOOST_CHECK_EQUAL(true, is_zero(b));
                        } else {
                                BOOST_CHECK_EQUAL(false, is_zero(b));
                                BOOST_CHECK_EQUAL(false, is_single(b));
                                BOOST_CHECK_EQUAL(true,  is_double(b));
                                BOOST_CHECK_EQUAL(true,  is_multiple(b));
                        }
                }
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsMultiple, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                for (auto j = 0; j < NUM_BITS<T>::value - i; ++j) {
                        T b = ((T(1) << i) - T(1)) << j;
                        switch (i) {
                        case 0:
                                BOOST_CHECK_EQUAL(true, is_zero(b));
                                break;
                        case 1:
                                BOOST_CHECK_EQUAL(true, is_single(b));
                                break;
                        case 2:
                                BOOST_CHECK_EQUAL(true, is_double(b));
                                BOOST_CHECK_EQUAL(true, is_multiple(b));
                                break;
                        default:                        
                                BOOST_CHECK_EQUAL(false, is_zero(b));
                                BOOST_CHECK_EQUAL(false, is_single(b));
                                BOOST_CHECK_EQUAL(false, is_double(b));
                                BOOST_CHECK_EQUAL(true,  is_multiple(b));
                        }
                }
        }
        BOOST_CHECK_EQUAL(true, is_multiple(T(~0)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IndexDeBruijn, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = T(1) << i;               
                BOOST_CHECK_EQUAL(i, index_DeBruijn(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IndexLookup, T, UnsignedIntegerTypes)
{
        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = T(1) << i;               
                BOOST_CHECK_EQUAL(i, index_lookup(b));
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountKernighan, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(0, count_Kernighan(T(0)));

        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = T(1) << i;
                BOOST_CHECK_EQUAL(1, count_Kernighan(b));                
        }

        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                for (auto j = 0; j < NUM_BITS<T>::value; ++j) {                                
                        T b = (T(1) << i) ^ (T(1) << j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, count_Kernighan(b));
                        else
                                BOOST_CHECK_EQUAL(2, count_Kernighan(b));
                }
        }

        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = (T(1) << i) - 1;
                BOOST_CHECK_EQUAL(i, count_Kernighan(b));
        }

        BOOST_CHECK_EQUAL(NUM_BITS<T>::value, count_Kernighan(T(~0)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CountLookup, T, UnsignedIntegerTypes)
{
        BOOST_CHECK_EQUAL(0, count_lookup(T(0)));

        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = T(1) << i;
                BOOST_CHECK_EQUAL(1, count_lookup(b));                
        }

        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                for (auto j = 0; j < NUM_BITS<T>::value; ++j) {                                
                        T b = (T(1) << i) ^ (T(1) << j);
                        if (i == j)
                                BOOST_CHECK_EQUAL(0, count_lookup(b));
                        else
                                BOOST_CHECK_EQUAL(2, count_lookup(b));
                }
        }

        for (auto i = 0; i < NUM_BITS<T>::value; ++i) {
                T b = (T(1) << i) - 1;
                BOOST_CHECK_EQUAL(i, count_lookup(b));
        }

        BOOST_CHECK_EQUAL(NUM_BITS<T>::value, count_lookup(T(~0)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace bit
}       // namespace dctl
