#include <algorithm>                            // any_of
#include <functional>                           // bind, _1
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // D000, D180, D360
#include <dctl/angle/transform.hpp>             // inverse, rotate, mirror
#include <dctl/group/transform.hpp>             // IsIdempotent

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleTransform)

using namespace std::placeholders;      // _1

auto const alpha = boost::counting_iterator<int>(angle::D000);
auto const omega = boost::counting_iterator<int>(angle::D360);

BOOST_AUTO_TEST_CASE(InverseIsIdempotentOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](int const& i){
                        return group::IsIdempotent()(std::bind(angle::inverse<int>, _1), i);
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate180IsIdempotentOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](int const& i){
                        return group::IsIdempotent()(std::bind(angle::rotate<int>, _1, D180), i);
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsIdempotentOnAllAnglePairs)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](int const& i){
                        return std::all_of(alpha, omega, [=](int const& j) {
                                return group::IsIdempotent()(std::bind(angle::mirror<int>, _1, j), i);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
