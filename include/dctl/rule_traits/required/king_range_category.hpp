#pragma once
#include <xstd/pp/tti/type.hpp> // XSTD_PP_TTI_TYPE
#include <cstddef>              // size_t

namespace dctl {

struct short_ranged_tag {};
struct  long_ranged_tag {};

XSTD_PP_TTI_TYPE(king_range_category, short_ranged_tag)

}       // namespace dctl
