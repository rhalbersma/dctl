#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_TYPE

namespace dctl {

struct stopped_promotion_tag {};
struct delayed_promotion_tag {};
struct passing_promotion_tag : delayed_promotion_tag {};

XSTD_PP_TTI_TYPE(promotion_category, stopped_promotion_tag)

}       // namespace dctl
