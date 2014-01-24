#include <limits>                       // numeric_Limits
#include <tuple>                        // get, tuple
#include <utility>                      // make_pair
#include <vector>                       // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/variants.hpp>      // Spanish
#include <dctl/utility/variadic.hpp>    // cartesian_product

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorValueSpanish)

using T = Value<rules::Spanish>;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
        T /* const */ v;
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

        for (auto&& c : cases) {
                auto const v = T{c};
                auto const p = std::get<0>(c);
                auto const k = std::get<1>(c);
                auto const n = p + k;

                BOOST_CHECK_EQUAL(v.size(), n);
                BOOST_CHECK_EQUAL(v.num_pawns(), p);
                BOOST_CHECK_EQUAL(v.num_kings(), k);
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

        for (auto&& c : cases) {
                auto const u = T{c};

                bool const incr[] = { false, true };
                for (auto&& i : incr) {
                        auto v = u;
                        v.increment(i);
                        BOOST_CHECK_EQUAL(v.size(), u.size() + 1);
                        if (i)
                                BOOST_CHECK_EQUAL(v.num_kings(), u.num_kings() + 1);
                        else
                                BOOST_CHECK_EQUAL(v.num_pawns(), u.num_pawns() + 1);
                }
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

        for (auto&& c : cases) {
                auto const u = T{c};

                bool const decr[] = { false, true };
                for (auto&& d : decr) {
                        auto v = u;
                        v.decrement(d);
                        BOOST_CHECK_EQUAL(v.size(), u.size() - 1);
                        if (d)
                                BOOST_CHECK_EQUAL(v.num_kings(), u.num_kings() - 1);
                        else
                                BOOST_CHECK_EQUAL(v.num_pawns(), u.num_pawns() - 1);
                }
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

        for (auto&& c : cases) {
                auto const u = T{c};
                auto v = u;
                BOOST_CHECK(u == v);
                BOOST_CHECK(v == u);

                bool const delta[] = { false, true };
                for (auto&& b : delta) {
                        auto x = u;

                        x.increment(b);
                        BOOST_CHECK(u != x);
                        BOOST_CHECK(x != u);

                        x.decrement(b);
                        BOOST_CHECK(u == x);
                        BOOST_CHECK(x == u);

                        auto y = u;

                        y.decrement(b);
                        BOOST_CHECK(u != y);
                        BOOST_CHECK(y != u);

                        y.increment(b);
                        BOOST_CHECK(u == y);
                        BOOST_CHECK(y == u);
                };
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

        for (auto&& c : cases) {
                auto const u = T{c};
                auto const v = u;
                BOOST_CHECK(u <= v);
                BOOST_CHECK(u >= v);
                BOOST_CHECK(v <= u);
                BOOST_CHECK(v >= u);

                bool const delta[] = { false, true };
                for (auto&& b : delta) {
                        auto x = u;

                        x.increment(b);
                        BOOST_CHECK(u < x);
                        BOOST_CHECK(x > u);

                        x.decrement(b);
                        BOOST_CHECK(u <= x);
                        BOOST_CHECK(u >= x);
                        BOOST_CHECK(x <= u);
                        BOOST_CHECK(x >= u);

                        auto y = u;

                        y.decrement(b);
                        BOOST_CHECK(u > y);
                        BOOST_CHECK(y < u);

                        y.increment(b);
                        BOOST_CHECK(u <= y);
                        BOOST_CHECK(u >= y);
                        BOOST_CHECK(y <= u);
                        BOOST_CHECK(y >= u);
                };
        }
}

BOOST_AUTO_TEST_CASE(Quantity)
{
        int const pawns[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        int const kings[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto&& c : cases) {
                auto const p = std::get<0>(c);
                auto const k = std::get<1>(c);

                // capturing one more pawn or one more king takes precedence
                BOOST_CHECK((T{p    , k - 1}) < (T{p    , k     }));
                BOOST_CHECK((T{p - 1, k    }) < (T{p    , k     }));
                BOOST_CHECK((T{p    , k    }) < (T{p + 1, k     }));
                BOOST_CHECK((T{p    , k    }) < (T{p    , k  + 1}));
        }
}

BOOST_AUTO_TEST_CASE(Quality)
{
        int const pawns[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        int const kings[] = { 1, 2, 3, std::numeric_limits<int>::max() / 2 };
        std::vector< std::tuple<int, int> > cases;
        variadic::cartesian_product(
                std::back_inserter(cases),
                std::make_pair(std::begin(pawns), std::end(pawns)),
                std::make_pair(std::begin(kings), std::end(kings))
        );

        for (auto&& c : cases) {
                auto const p = std::get<0>(c);
                auto const k = std::get<1>(c);

                // capturing one more king for one less pawn takes precedence
                BOOST_CHECK((T{p + 1, k - 1}) < (T{p    , k     }));
                BOOST_CHECK((T{p    , k    }) < (T{p - 1, k  + 1}));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
