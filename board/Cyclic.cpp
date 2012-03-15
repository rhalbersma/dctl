#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include "../../src/board/Cyclic.hpp"
#include "../../src/group/axioms.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestCyclic)

BOOST_AUTO_TEST_CASE(Axioms)
{
        BOOST_CHECK(group::is_axioms<C1>::value);
        BOOST_CHECK(group::is_axioms<C2>::value);
        BOOST_CHECK(group::is_axioms<C4>::value);
        BOOST_CHECK(group::is_axioms<C8>::value);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
