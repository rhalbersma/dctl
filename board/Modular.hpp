#pragma once
#include <boost/mpl/comparison.hpp>     // equal_to
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/modulus.hpp>        // modulus
#include "Degrees.hpp"

namespace dctl {

// numerical metafunction
template<typename T>
struct mod_360
: 
        boost::mpl::modulus< 
                boost::mpl::int_<T::value>, 
                boost::mpl::int_<degrees::D360> 
        > 
{};

// predicate metafunction
template<typename T>
struct is_div_090
: 
        boost::mpl::equal_to<
                boost::mpl::modulus< 
                        boost::mpl::int_<T::value>, 
                        boost::mpl::int_<degrees::D090> 
                >,
                boost::mpl::int_<0>
        >
{};

}       // namespace dctl
