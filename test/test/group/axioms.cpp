#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <dctl/group/axioms.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>	        // C1, C2, C4, C8

namespace dctl {
namespace group {
namespace axioms {

BOOST_AUTO_TEST_SUITE(GroupAxioms)

using Groups = boost::mpl::vector
<
        C1, C2, C4, C8
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRealized, T, Groups)
{
        BOOST_MPL_ASSERT(( is_realized<T> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace axioms
}       // namespace group
}       // namespace dctl
