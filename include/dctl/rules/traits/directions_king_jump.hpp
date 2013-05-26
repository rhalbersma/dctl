#pragma once
#include <type_traits>
#include <boost/mpl/eval_if.hpp>                                // eval_if
#include <dctl/rules/traits/orthogonality_king_jump.hpp>
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct directions_king_jump
:
        boost::mpl::eval_if<
                std::is_same< typename
                        orthogonality_king_jump<Rules>::type,
                        orthogonality::none
                >,
                directions::diag,
                directions::all
        >::type
{};

}       // namespace rules
}       // namespace dctl
