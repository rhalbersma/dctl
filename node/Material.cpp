#include <boost/test/unit_test.hpp>             // BOOST_CHECK_EQUAL 
#include "../../src/node/Material.hpp"
#include "../../src/bit/Bit.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestNode)

BOOST_AUTO_TEST_CASE(MaterialComposition)
{
        BOOST_CHECK(bit::is_zero(Material::none & Material::pawn));
        BOOST_CHECK(bit::is_zero(Material::none & Material::king));
        BOOST_CHECK(bit::is_zero(Material::pawn & Material::king));

        BOOST_CHECK(bit::is_single(Material::both & Material::pawn));
        BOOST_CHECK(bit::is_single(Material::both & Material::king));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
