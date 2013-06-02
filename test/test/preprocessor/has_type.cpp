#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT, BOOST_MPL_ASSERT_NOT
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/preprocessor/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE

namespace dctl {

BOOST_AUTO_TEST_SUITE(PreprocessorHasType)

DCTL_PP_DEFINE_HAS_TYPE(type)

struct empty {};

struct dummy
{
        typedef void type;
};

BOOST_AUTO_TEST_CASE(Empty)
{
        BOOST_MPL_ASSERT_NOT(( has_type<empty> ));
}

BOOST_AUTO_TEST_CASE(Dummy)
{
        BOOST_MPL_ASSERT(( has_type<dummy> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
