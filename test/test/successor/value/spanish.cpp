#include <limits>                       // numeric_Limits
#include <tuple>                        // get, tuple
#include <utility>                      // make_pair
#include <vector>                       // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/variants.hpp>      // Spanish
#include <dctl/utility/variadic.hpp>    // cartesian_product

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(TestSpanish)

typedef Value<rules::Spanish> value_type;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
        value_type const v;
        BOOST_CHECK_EQUAL(v.size(), 0);
}

BOOST_AUTO_TEST_CASE(TupleConstructor)
{
        int const pawns[] = { 0, 1, 2, std::numeric_limits<int>::max() / 2 };
        int const kings[] = { 0, 1, 2, std::numeric_limits<int>::max() / 2 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto const& c: cases) {
                auto const v = value_type { c };
                auto const n = std::get<0>(c) + std::get<1>(c);
                BOOST_CHECK_EQUAL(v.size(), n);
        }
}

BOOST_AUTO_TEST_CASE(Increment)
{
        int const pawns[] = { 0, 1, 2, std::numeric_limits<int>::max() / 2 - 1 };
        int const kings[] = { 0, 1, 2, std::numeric_limits<int>::max() / 2 - 1 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto const& c: cases) {
                auto v = value_type { c };
                auto const n = v.size();

                v.increment(false);
                BOOST_CHECK_EQUAL(v.size(), n + 1);
                v.increment(true);
                BOOST_CHECK_EQUAL(v.size(), n + 2);
        }
}

BOOST_AUTO_TEST_CASE(Decrement)
{
        int const pawns[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        int const kings[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto const& c: cases) {
                auto v = value_type { c };
                auto const n = v.size();

                v.decrement(false);
                BOOST_CHECK_EQUAL(v.size(), n - 1);
                v.decrement(true);
                BOOST_CHECK_EQUAL(v.size(), n - 2);
        }
}

BOOST_AUTO_TEST_CASE(Equal)
{
        int const pawns[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        int const kings[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto const& c: cases) {
                auto const u = value_type { c };
                auto v = u;
                BOOST_CHECK(u == v);
                BOOST_CHECK(v == u);

                v.increment(false);
                BOOST_CHECK(u != v);
                BOOST_CHECK(v != u);

                auto w = u;
                w.increment(true);
                BOOST_CHECK(u != w);
                BOOST_CHECK(w != u);

                auto x = u;
                x.decrement(false);
                BOOST_CHECK(u != x);
                BOOST_CHECK(x != u);

                auto y = u;
                y.decrement(true);
                BOOST_CHECK(u != y);
                BOOST_CHECK(y != u);
        }
}

BOOST_AUTO_TEST_CASE(Less)
{
        int const pawns[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        int const kings[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto const& c: cases) {
                auto const u = value_type { c };
                auto v = u;
                BOOST_CHECK(u <= v);
                BOOST_CHECK(v >= u);

                v.increment(false);
                BOOST_CHECK(u < v);
                BOOST_CHECK(v > u);

                auto w = u;
                w.increment(true);
                BOOST_CHECK(u < w);
                BOOST_CHECK(w > u);

                auto x = u;
                x.decrement(false);
                BOOST_CHECK(u > x);
                BOOST_CHECK(x < u);

                auto y = u;
                y.decrement(true);
                BOOST_CHECK(u > y);
                BOOST_CHECK(y < u);
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
