#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_TYPE

namespace dctl {

struct promotion_sur_place_tag
{};

struct promotion_apres_fini_tag
{};

struct promotion_en_passant_tag
:
        promotion_apres_fini_tag
{};

XSTD_PP_TTI_TYPE(promotion_category, promotion_sur_place_tag)

}       // namespace dctl
