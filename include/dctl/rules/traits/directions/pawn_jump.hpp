#pragma once
#include <dctl/rules/traits/is_backward_pawn_jump.hpp>
#include <dctl/rules/traits/is_orthogonal_jump.hpp>
#include <dctl/rules/types/directions/all.hpp>
#include <dctl/rules/types/directions/diag.hpp>
#include <dctl/rules/types/directions/up.hpp>
#include <boost/mpl/eval_if.hpp>

namespace dctl {
namespace rules {
namespace directions {

template<class Rules>
struct pawn_jump
:
        boost::mpl::eval_if<
                is_backward_pawn_jump_t<Rules>,
                boost::mpl::eval_if<
                        is_orthogonal_jump_t<Rules>,
                        directions::all,
                        directions::diag
                >,
                directions::up
        >::type
{};

}       // namespace directions
}       // namespace rules
}       // namespace dctl
