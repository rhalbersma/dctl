#pragma once
#include <type_traits>                  // is_base_of
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/lambda.hpp>         // lambda, _1, _2
#include <boost/mpl/logical.hpp>        // and_, true_

namespace dctl {

template
<
        typename Base,
        typename DerivedVector          // Boost.MPL sequence 
>
struct is_base_of_all
:
        boost::mpl::fold<
                DerivedVector,
                boost::mpl::true_,
                boost::mpl::lambda<
                        boost::mpl::and_<
                                boost::mpl::_1,
                                std::is_base_of< Base, boost::mpl::_2 >
                        >
                >                
        >
{};

}       // namespace dctl
