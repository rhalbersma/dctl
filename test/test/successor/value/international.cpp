#include <limits>                               // numeric_limits
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/rules/variants.hpp>              // International, Damme, Hoogland, Killer
#include <dctl/successor/value.hpp>             // Value (primary template)

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(TestInternational)

// rules variants with the international capture precedence
typedef boost::mpl::vector<
        rules::International /*, rules::Damme, rules::Hoogland, rules::Killer*/
> PrecedenceInternational;

BOOST_AUTO_TEST_CASE_TEMPLATE(DefaultConstructor, T, PrecedenceInternational)
{
        Value<T> v;
        BOOST_CHECK_EQUAL(v.size(), 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(CompleteConstructor, T, PrecedenceInternational)
{
        int const pieces[] = { 0, 1, 2, std::numeric_limits<int>::max() };

        for (auto const& p: pieces) {
                auto const v = Value<T> { p };
                BOOST_CHECK_EQUAL(v.size(), p);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Increment, T, PrecedenceInternational)
{
        int const pieces[] = { 0, 1, 2, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                auto v = Value<T> { p };
                v.increment();
                BOOST_CHECK_EQUAL(v.size(), p + 1);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Decrement, T, PrecedenceInternational)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() };

        for (auto const& p: pieces) {
                auto v = Value<T> { p };
                v.decrement();
                BOOST_CHECK_EQUAL(v.size(), p - 1);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Equal, T, PrecedenceInternational)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                auto const u = Value<T> { p };
                auto const v = u;
                BOOST_CHECK(u == v);
                BOOST_CHECK(v == u);

                auto x = u;

                x.increment();
                BOOST_CHECK(u != x);
                BOOST_CHECK(x != u);

                x.decrement();
                BOOST_CHECK(u == x);
                BOOST_CHECK(x == u);

                auto y = u;

                y.decrement();
                BOOST_CHECK(u != y);
                BOOST_CHECK(y != u);

                y.increment();
                BOOST_CHECK(u == y);
                BOOST_CHECK(y == u);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Less, T, PrecedenceInternational)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                auto const u = Value<T> { p };
                auto const v = u;
                BOOST_CHECK(u <= v);
                BOOST_CHECK(v >= u);

                auto x = u;

                x.increment();
                BOOST_CHECK(u < x);
                BOOST_CHECK(x > u);

                x.decrement();
                BOOST_CHECK(u <= x);
                BOOST_CHECK(u >= x);
                BOOST_CHECK(x <= u);
                BOOST_CHECK(x >= u);

                auto y = u;

                y.decrement();
                BOOST_CHECK(u > y);
                BOOST_CHECK(y < u);

                y.increment();
                BOOST_CHECK(u <= y);
                BOOST_CHECK(u >= y);
                BOOST_CHECK(y <= u);
                BOOST_CHECK(y >= u);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Quantity, T, PrecedenceInternational)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                // capturing one more piece takes precedence
                BOOST_CHECK(Value<T>(p - 1) < Value<T>(p    ));
                BOOST_CHECK(Value<T>(p    ) < Value<T>(p + 1));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
