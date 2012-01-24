#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>

#include <type_traits>                          // std::is_same
#include <boost/mpl/list.hpp>                   // boost::mpl::list
#include "Transform.hpp"
#include "../../src/board/Angle.hpp"
#include "../../src/board/Degrees.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestAngle)

typedef boost::mpl::list<
        angle<degrees::D000>, 
        angle<degrees::D045>, 
        angle<degrees::D090>, 
        angle<degrees::D135>, 
        angle<degrees::D180>, 
        angle<degrees::D225>, 
        angle<degrees::D270>, 
        angle<degrees::D315>
> AngleList;

template<typename A1, typename A2>
struct is_zero_angle_commutator
:
        std::is_same<
                typename rotate< A1, A2 >::type,
                typename rotate< A2, A1 >::type
        >              
{};

template<typename T1>
struct check_zero_angle_commutator
{
        template<typename T2>
        void operator()(boost::mpl::identity<T2>)
        {
                BOOST_CHECK((is_zero_angle_commutator< T1, T2 >::value));
        }
};

BOOST_AUTO_TEST_CASE_TEMPLATE(ZeroAngleCommutator, T, AngleList)
{
        boost::mpl::for_each<AngleList, boost::mpl::make_identity<> >(
                check_zero_angle_commutator<T>() 
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsInverseRotate, T, AngleList)
{
        // the identity and rotations over 180 degrees are their own inverse
        BOOST_CHECK((is_inverse_rotate<T, angle<degrees::D000>, angle<degrees::D000> >::value));
        BOOST_CHECK((is_inverse_rotate<T, angle<degrees::D180>, angle<degrees::D180> >::value));

        // left and right rotations are each other's inverse
        BOOST_CHECK((is_inverse_rotate<T, angle<degrees::L045>, angle<degrees::R045> >::value));
        BOOST_CHECK((is_inverse_rotate<T, angle<degrees::L090>, angle<degrees::R090> >::value));
        BOOST_CHECK((is_inverse_rotate<T, angle<degrees::L135>, angle<degrees::R135> >::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSquareRotate, T, AngleList)
{
        BOOST_CHECK((is_square_rotate<T, angle<degrees::L090>, angle<degrees::L045> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::L090>, angle<degrees::R135> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::R090>, angle<degrees::R045> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::R090>, angle<degrees::L135> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D180>, angle<degrees::L090> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D180>, angle<degrees::R090> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D000>, angle<degrees::D180> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D000>, angle<degrees::D000> >::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InversePositive, T, AngleList)
{
        BOOST_CHECK_GE(inverse<T>::value, 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(InverseIdemPotent, T, AngleList)
{
        BOOST_CHECK((
                std::is_same<
                        T, 
                        typename inverse< inverse< T > >::type
                >::value
        ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(MirrorUpIdemPotent, T, AngleList)
{
        BOOST_CHECK((
                std::is_same<
                        T, 
                        typename mirror_up< mirror_up< T > >::type
                >::value
        ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(MirrorDownIdemPotent, T, AngleList)
{
        BOOST_CHECK((
                std::is_same<
                        T, 
                        typename mirror_down< mirror_down< T > >::type
                >::value
        ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
