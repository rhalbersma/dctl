#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply

namespace dctl {
namespace mpl {

template<typename F, typename X>
struct is_idempotent
:
        std::is_same< typename 
                boost::mpl::apply< 
                        F, typename 
                        boost::mpl::apply< F, X >::type 
                >::type,
                X
        >              
{};

}       // namespace mpl
}       // namespace dctl
