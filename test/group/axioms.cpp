#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include "../../src/group/axioms.hpp"
#include "../../src/angle/Cyclic.hpp"

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(TestAxioms)

BOOST_AUTO_TEST_CASE(IsAxioms)
{
        BOOST_MPL_ASSERT(( is_axioms<angle::C1> ));
        BOOST_MPL_ASSERT(( is_axioms<angle::C2> ));
        BOOST_MPL_ASSERT(( is_axioms<angle::C4> ));
        BOOST_MPL_ASSERT(( is_axioms<angle::C8> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
