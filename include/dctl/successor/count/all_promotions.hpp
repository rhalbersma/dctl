#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/pawn_promotions.hpp>     // count (pawn moves specialization)
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/pieces/pieces.hpp>                // all, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece promotions
template<bool Color>
struct count<Color, pieces::all, select::promotions>
:
        count<Color, pieces::pawn, select::promotions>
{};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
