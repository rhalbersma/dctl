#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_TYPE
#include <cstddef>              // size_t

namespace dctl {

struct short_ranged_tag {};
struct  long_ranged_tag {};

XSTD_PP_TTI_TYPE_NO_DEFAULT(king_range_category)

}       // namespace dctl
