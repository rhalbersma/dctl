#include <type_traits>                          // false_type, true_type
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT, BOOST_MPL_ASSERT_NOT
#include <boost/mpl/eval_if.hpp>                // eval_if
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/preprocessor/sfinae/has_type.hpp>       // DCTL_PP_DEFINE_HAS_TYPE
#include <dctl/preprocessor/sfinae/get_type.hpp>       // DCTL_PP_DEFINE_GET_TYPE

namespace dctl {

BOOST_AUTO_TEST_SUITE(PreprocessorGetType)

DCTL_PP_DEFINE_HAS_TYPE(U)
DCTL_PP_DEFINE_GET_TYPE(U)

struct empty {};

struct dummy
{
        using U = std::true_type;
};

template<class T>
struct extract_type
:
        boost::mpl::eval_if<
                has_type_U<T>,
                get_type_U<T>,
                std::false_type
        >::type
{};

BOOST_AUTO_TEST_CASE(Empty)
{
        BOOST_MPL_ASSERT_NOT(( extract_type<empty> ));
}

BOOST_AUTO_TEST_CASE(Dummy)
{
        BOOST_MPL_ASSERT(( extract_type<dummy> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
