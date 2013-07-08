#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/vector.hpp>                 // vector
#include <dctl/rules/variants.hpp>              // Checkers, Czech, Pool, Thai
#include <dctl/successor/value.hpp>             // Value (primary template)

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorValueDefault)

// rules variants without capture precedence
using PrecedenceNone = boost::mpl::vector
<
        rules::Checkers, rules::Czech, rules::Pool, rules::Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Equal, T, PrecedenceNone)
{
        Value<T> /* const */ v, w;

        // all captures are equal
        BOOST_CHECK(v == w);
        BOOST_CHECK(w == v);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Less, T, PrecedenceNone)
{
        Value<T> /* const */ v, w;

        // all captures are equivalent
        BOOST_CHECK(v <= w);
        BOOST_CHECK(v >= w);
        BOOST_CHECK(w <= v);
        BOOST_CHECK(w >= v);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
