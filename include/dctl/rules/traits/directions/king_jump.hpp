#pragma once
#include <type_traits>                                          // is_same
#include <boost/mpl/eval_if.hpp>                                // eval_if
#include <dctl/rules/traits/orthogonality/king_jump.hpp>        // king_jump
#include <dctl/rules/types/directions.hpp>                      // diag, all
#include <dctl/rules/types/orthogonality.hpp>                   // none

namespace dctl {
namespace rules {
namespace directions {

template<class Rules>
struct king_jump
:
        boost::mpl::eval_if<
                std::is_same< typename
                        orthogonality::king_jump<Rules>::type,
                        orthogonality::none
                >,
                diag,
                all
        >::type
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
