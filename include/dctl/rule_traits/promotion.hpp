#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT
#include <type_traits>          // conditional, false_type, true_type

namespace dctl {

XSTD_PP_TTI_CONSTANT(is_passing_promotion, false)

struct stopped_promotion_tag : std::false_type {};
struct passing_promotion_tag : std::true_type {};

template<class Rules>
using promotion_category_t = std::conditional_t<
        is_passing_promotion_v<Rules>,
        passing_promotion_tag,
        stopped_promotion_tag
>;

}       // namespace dctl
