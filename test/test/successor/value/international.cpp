#include <limits>                       // numeric_limits
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/variants.hpp>      // International

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(TestInternational)

typedef Value<rules::International> value_type;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
        value_type const v;
        BOOST_CHECK_EQUAL(v.size(), 0);
}

BOOST_AUTO_TEST_CASE(CompleteConstructor)
{
        int const pieces[] = { 0, 1, 2, std::numeric_limits<int>::max() };

        for (auto const& p: pieces) {
                auto const v = value_type { p };
                BOOST_CHECK_EQUAL(v.size(), p);
        }
}

BOOST_AUTO_TEST_CASE(Increment)
{
        int const pieces[] = { 0, 1, 2, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                auto v = value_type { p };
                v.increment();
                BOOST_CHECK_EQUAL(v.size(), p + 1);
        }
}

BOOST_AUTO_TEST_CASE(Decrement)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() };

        for (auto const& p: pieces) {
                auto v = value_type { p };
                v.decrement();
                BOOST_CHECK_EQUAL(v.size(), p - 1);
        }
}

BOOST_AUTO_TEST_CASE(Equal)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                auto const u = value_type { p };
                auto v = u;
                BOOST_CHECK(u == v);
                BOOST_CHECK(v == u);

                v.increment();
                BOOST_CHECK(u != v);
                BOOST_CHECK(v != u);

                auto w = u;
                w.decrement();
                BOOST_CHECK(u != w);
                BOOST_CHECK(w != u);
        }
}

BOOST_AUTO_TEST_CASE(Less)
{
        int const pieces[] = { 1, 2, 3, std::numeric_limits<int>::max() - 1 };

        for (auto const& p: pieces) {
                auto const u = value_type { p };
                auto v = u;
                BOOST_CHECK(u <= v);
                BOOST_CHECK(v >= u);

                v.increment();
                BOOST_CHECK(u < v);
                BOOST_CHECK(v > u);

                auto w = u;
                w.decrement();
                BOOST_CHECK(u > w);
                BOOST_CHECK(w < u);
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
