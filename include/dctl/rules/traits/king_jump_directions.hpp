#pragma once
#include <type_traits>
#include <boost/mpl/eval_if.hpp>                                // eval_if
#include <dctl/rules/traits/king_jump_orthogonality.hpp>
#include <dctl/rules/enum.hpp>

namespace dctl {
namespace rules {

template<class Rules>
struct king_jump_directions
:
        boost::mpl::eval_if<
                std::is_same< typename
                        king_jump_orthogonality<Rules>::type,
                        orthogonality::none
                >,
                directions::diag,
                directions::all
        >::type
{};

}       // namespace rules
}       // namespace dctl
