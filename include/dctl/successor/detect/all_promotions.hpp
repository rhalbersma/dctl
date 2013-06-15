#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/pawn_promotions.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/pieces/pieces.hpp>                // all, pawn

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, class Range>
struct detect<Color, pieces::all, select::promotions, Range>
:
        detect<Color, pieces::pawn, select::promotions, Range>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
