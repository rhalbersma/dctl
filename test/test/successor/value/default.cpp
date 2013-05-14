#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/rules/variants.hpp>
#include <dctl/successor/value.hpp>

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(TestDefault)

typedef boost::mpl::vector<
        rules::Checkers, rules::Czech, rules::Pool, rules::Thai
> RulesVariants;

BOOST_AUTO_TEST_CASE_TEMPLATE(Equal, T, RulesVariants)
{
        Value<T> const v, w;

        BOOST_CHECK(v == w);
        BOOST_CHECK(w == v);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Less, T, RulesVariants)
{
        Value<T> const v, w;

        BOOST_CHECK(v <= w);
        BOOST_CHECK(w >= v);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
