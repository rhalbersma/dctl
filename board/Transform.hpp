#pragma once
#include <type_traits>                          // std::is_same
#include <boost/config.hpp>                     // BOOST_STATIC_CONSTANT
#include "../../src/board/Angle.hpp"
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Transform.hpp"

#include <boost/mpl/comparison.hpp>
#include <boost/mpl/int.hpp>

namespace dctl {

template<typename T, typename A1, typename A2>
struct fold_rotate
:
        rotate< rotate< T, A2 >, A1 >
{};

template<typename T, typename A1, typename A2>
struct is_zero_commutator
:
        std::is_same<
                typename fold_rotate<T, A1, A2>::type,
                typename fold_rotate<T, A2, A1>::type
        >
{};

template<typename T, typename A1, typename A2>
struct is_inverse_rotate
:
        //std::is_same<
        boost::mpl::equal_to<
                boost::mpl::int_<T::value>, 
                boost::mpl::int_<fold_rotate<T, A1, A2 >::value>
        >
{};

template<typename T, typename A1, typename A2>
struct is_square_rotate
:
        std::is_same<
                typename rotate<T, A1>::type, 
                typename fold_rotate<T, A2, A2>::type
        >
{};

}       // namespace dctl
