#include <type_traits>                                  // std::is_same
#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>                           // boost::mpl::list
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Transform.hpp"
#include "Transform.hpp"

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestDimensions)

typedef boost::mpl::list<
        Dimensions< 4,  4>,
        Dimensions< 6,  6>,
        Dimensions< 8,  8>,
        Dimensions<10, 10>,
        Dimensions<10,  8, true>,
        Dimensions<11, 10, true>,
        Dimensions<12, 10, true>
> DimensionsList;

typedef boost::mpl::list<
        angle<degrees::D000>, 
        angle<degrees::D090>, 
        angle<degrees::D180>, 
        angle<degrees::D270> 
> AngleList;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsCommuteRotate, T, DimensionsList)
{
        // all rotations commute with themselves
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::D000>, angle<degrees::D000> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::L090>, angle<degrees::L090> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::R090>, angle<degrees::R090> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::D180>, angle<degrees::D180> >::value));

        // all rotations commute with the identity
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::D000>, angle<degrees::R090> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::D000>, angle<degrees::L090> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::D000>, angle<degrees::D180> >::value));

        // non-trivial commutators
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::L090>, angle<degrees::R090> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::L090>, angle<degrees::D180> >::value));
        BOOST_CHECK((is_zero_commutator<T, angle<degrees::R090>, angle<degrees::D180> >::value));
}
/*
template<typename T>
struct check_inverse_rotate
{
        template<int N>
        void operator()(boost::mpl::identity< angle<N> >)
        {
                BOOST_CHECK((is_inverse_rotate< T, angle<N>, inverse< angle<N> > >::value));
        }
};

BOOST_AUTO_TEST_CASE_TEMPLATE(IsInverseRotate, T, DimensionsList)
{
        boost::mpl::for_each<AngleList, boost::mpl::make_identity<> >(
                check_inverse_rotate<T>() 
        );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSquareRotate, T, DimensionsList)
{
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D180>, angle<degrees::L090> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D180>, angle<degrees::R090> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D000>, angle<degrees::D180> >::value));
        BOOST_CHECK((is_square_rotate<T, angle<degrees::D000>, angle<degrees::D000> >::value));
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
