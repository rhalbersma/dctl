#pragma once
#include <dctl/rules/traits/is_long_ranged_king.hpp>
#include <dctl/rules/types/range/distance_1.hpp>
#include <dctl/rules/types/range/distance_N.hpp>
#include <boost/mpl/eval_if.hpp>

namespace dctl {
namespace rules {
namespace range {

template<class Rules>
struct king
:
        boost::mpl::eval_if<
                is_long_ranged_king_t<Rules>,
                distance_N,
                distance_1
        >
{};

}       // namespace range
}       // namespace rules
}       // namespace dctl
