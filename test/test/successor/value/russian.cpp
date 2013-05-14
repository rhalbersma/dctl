#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/variants.hpp>      // Russian

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(TestRussian)

typedef Value<rules::Russian> value_type;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
        value_type const v;
        BOOST_CHECK(!v.is_promotion());
}

BOOST_AUTO_TEST_CASE(CompleteConstructor)
{
        auto const v = value_type { false };
        BOOST_CHECK(!v.is_promotion());

        auto const w = value_type { true };
        BOOST_CHECK(w.is_promotion());
}

BOOST_AUTO_TEST_CASE(TogglePromotion)
{
        auto v = value_type { false };
        v.toggle_promotion();
        BOOST_CHECK(v.is_promotion());

        auto w = value_type { true };
        w.toggle_promotion();
        BOOST_CHECK(!w.is_promotion());
}

BOOST_AUTO_TEST_CASE(Equal)
{
        auto const u = value_type { false };
        auto const v = u;
        BOOST_CHECK(u == v);
        BOOST_CHECK(v == u);

        auto const w = value_type { true };
        auto const x = w;
        BOOST_CHECK(w == x);
        BOOST_CHECK(x == w);

        BOOST_CHECK(u == w);
        BOOST_CHECK(w == u);
}

BOOST_AUTO_TEST_CASE(Less)
{
        auto const u = value_type { false };
        auto const v = u;
        BOOST_CHECK(u <= v);
        BOOST_CHECK(v >= u);

        auto const w = value_type { true };
        auto const x = w;
        BOOST_CHECK(w <= x);
        BOOST_CHECK(x >= w);

        BOOST_CHECK(u <= w);
        BOOST_CHECK(w >= u);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
