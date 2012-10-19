#include <string>                       // string
#include <type_traits>                  // is_base_of, is_pod
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT, BOOST_MPL_ASSERT_NOT
#include <boost/mpl/placeholders.hpp>   // _1
#include <boost/mpl/vector.hpp>         // vector
#include "../../src/mpl/algorithm.hpp"  // all_of, none_of, any_of

namespace dctl {
namespace mpl {

BOOST_AUTO_TEST_SUITE(TestMPLAlgorithm)

typedef int P;
typedef std::string N;

typedef boost::mpl::vector<P> pod_types;
typedef boost::mpl::vector<P, N> mixed_types;
typedef boost::mpl::vector<N> non_pod_types;

struct B {};
struct D: B {};
struct E {};

typedef boost::mpl::vector<B, D> derived_types;
typedef boost::mpl::vector<B, D, E> both_types;
typedef boost::mpl::vector<E> independent_types;

BOOST_AUTO_TEST_CASE(TestAllOf)
{
        BOOST_MPL_ASSERT    (( all_of< pod_types,     std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< mixed_types,   std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< non_pod_types, std::is_pod< boost::mpl::_1 > > ));

        BOOST_MPL_ASSERT    (( all_of< derived_types,     std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< both_types,        std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( all_of< independent_types, std::is_base_of< B, boost::mpl::_1 > > ));
}

BOOST_AUTO_TEST_CASE(TestNoneOf)
{
        BOOST_MPL_ASSERT_NOT(( none_of< pod_types,     std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( none_of< mixed_types,   std::is_pod< boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT    (( none_of< non_pod_types, std::is_pod< boost::mpl::_1 > > ));

        BOOST_MPL_ASSERT_NOT(( none_of< derived_types,     std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT_NOT(( none_of< both_types,        std::is_base_of< B, boost::mpl::_1 > > ));
        BOOST_MPL_ASSERT    (( none_of< independent_types, std::is_base_of< B, boost::mpl::_1 > > ));
}

BOOST_AUTO_TEST_CASE(TestAnyOf)
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
