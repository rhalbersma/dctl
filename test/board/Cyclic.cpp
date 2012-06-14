#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include "../../src/board/Cyclic.hpp"
#include "../../src/group/axioms.hpp"

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(TestCyclic)

BOOST_AUTO_TEST_CASE(Axioms)
{
        BOOST_MPL_ASSERT(( group::is_axioms<C1> ));
        BOOST_MPL_ASSERT(( group::is_axioms<C2> ));
        BOOST_MPL_ASSERT(( group::is_axioms<C4> ));
        BOOST_MPL_ASSERT(( group::is_axioms<C8> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
