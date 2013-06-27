#include <string>                       // string
#include <type_traits>                  // is_base_of, is_pod
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT, BOOST_MPL_ASSERT_NOT
#include <boost/mpl/placeholders.hpp>   // _1
#include <boost/mpl/vector.hpp>         // vector
#include <dctl/mpl/algorithm.hpp>       // all_of, none_of, any_of

namespace dctl {
namespace mpl {

BOOST_AUTO_TEST_SUITE(MPLAlgorithm)

using P = int;
using N = std::string;

using pod_types = boost::mpl::vector<P>;
using mixed_types = boost::mpl::vector<P, N>;
using non_pod_types = boost::mpl::vector<N>;

struct B {};
struct D: B {};
struct S {};

using derived_types = boost::mpl::vector<B, D>;
using both_types = boost::mpl::vector<B, D, S>;
using independent_types = boost::mpl::vector<S>;

BOOST_AUTO_TEST_CASE(AllOf)
{
        BOOST_MPL_ASSERT    (( all_of< pod_types,     std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< mixed_types,   std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< non_pod_types, std::is_pod< boost::mpl::_1 > > ));

        BOOST_MPL_ASSERT    (( all_of< derived_types,     std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< both_types,        std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< independent_types, std::is_base_of< B, boost::mpl::_1 > > ));
}

BOOST_AUTO_TEST_CASE(NoneOf)
{
        BOOST_MPL_ASSERT_NOT(( none_of< pod_types,     std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( none_of< mixed_types,   std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT    (( none_of< non_pod_types, std::is_pod< boost::mpl::_1 > > ));

        BOOST_MPL_ASSERT_NOT(( none_of< derived_types,     std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( none_of< both_types,        std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT    (( none_of< independent_types, std::is_base_of< B, boost::mpl::_1 > > ));
}

BOOST_AUTO_TEST_CASE(AnyOf)
{
        BOOST_MPL_ASSERT    (( any_of< pod_types,     std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT    (( any_of< mixed_types,   std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( any_of< non_pod_types, std::is_pod< boost::mpl::_1 > > ));

        BOOST_MPL_ASSERT    (( any_of< derived_types,     std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT    (( any_of< both_types,        std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( any_of< independent_types, std::is_base_of< B, boost::mpl::_1 > > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace mpl
}       // namespace dctl
