#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/variants.hpp>      // Russian

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorValueRussian)

typedef Value<rules::Russian> value_type;

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
        value_type const v;
        BOOST_CHECK(!v.is_promotion());
}

BOOST_AUTO_TEST_CASE(CompleteConstructor)
{
        bool const promotion[] = { false, true };

        for (auto const& p: promotion ) {
                auto const v = value_type { p };
                BOOST_CHECK_EQUAL(v.is_promotion(), p);
        }
}

BOOST_AUTO_TEST_CASE(TogglePromotion)
{
        bool const promotion[] = { false, true };

        for (auto const& p: promotion ) {
                auto v = value_type { p };
                auto const b = v.is_promotion();

                v.toggle_promotion();
                BOOST_CHECK_NE(v.is_promotion(), b);

                v.toggle_promotion();
                BOOST_CHECK_EQUAL(v.is_promotion(), b);
        }
}

BOOST_AUTO_TEST_CASE(Equal)
{
        bool const promotion[] = { false, true };

        for (auto const& p: promotion ) {
                auto v = value_type { p };
                auto const w = v;
                BOOST_CHECK(v == w);
                BOOST_CHECK(w == v);

                v.toggle_promotion();
                BOOST_CHECK(v == w);
                BOOST_CHECK(w == v);
        }
}

BOOST_AUTO_TEST_CASE(Less)
{
        bool const promotion[] = { false, true };

        for (auto const& p: promotion ) {
                auto v = value_type { p };
                auto const w = v;
                BOOST_CHECK(v <= w);
                BOOST_CHECK(v >= w);
                BOOST_CHECK(w <= v);
                BOOST_CHECK(w >= v);

                v.toggle_promotion();
                BOOST_CHECK(v <= w);
                BOOST_CHECK(v >= w);
                BOOST_CHECK(w <= v);
                BOOST_CHECK(w >= v);
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
