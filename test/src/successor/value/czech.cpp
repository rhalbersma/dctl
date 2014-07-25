#include <dctl/rules/variants.hpp>      // Czech
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorValueCzech)

using T = Value<rules::Czech>;
static constexpr auto d = T{};
static constexpr auto n = T{false};
static constexpr auto y = T{true };

BOOST_AUTO_TEST_CASE(IsEqualityComparable)
{
        static_assert(d == n, "");
        static_assert(n == d, "");
        static_assert(n == n, "");
        static_assert(y == y, "");

        static_assert(!(n == y), "");
        static_assert(!(y == n), "");

        static_assert(!(d == y), "");
        static_assert(!(y == d), "");

        // Boost.Operators does not provide constexpr

        BOOST_CHECK(n != y);
        BOOST_CHECK(y != n);

        BOOST_CHECK(d != y);
        BOOST_CHECK(y != d);
}

BOOST_AUTO_TEST_CASE(IsStrictWeakOrder)
{
        static_assert(!(n < n), "");
        static_assert(!(y < y), "");

        static_assert(  n < y , "");
        static_assert(!(y < n), "");

        // Boost.Operators does not provide constexpr

        BOOST_CHECK(!(n >  n));
        BOOST_CHECK(  n >= n );
        BOOST_CHECK(  n <= n );
        BOOST_CHECK(!(y >  y));
        BOOST_CHECK(  y >= y );
        BOOST_CHECK(  y <= y );

        BOOST_CHECK(  y >  n );
        BOOST_CHECK(!(n >  y));
        BOOST_CHECK(  y >= n );
        BOOST_CHECK(  n <= y );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
