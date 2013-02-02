#pragma once
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/detect/primary.hpp>
#include <dctl/successor/detect/both_jumps.hpp>
#include <dctl/successor/detect/both_promotions.hpp>
#include <dctl/successor/detect/king_jumps.hpp>
// there are no king promotions
#include <dctl/successor/detect/pawn_jumps.hpp>
#include <dctl/successor/detect/pawn_promotions.hpp>
#include <dctl/successor/select/conversion.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors detection
template<bool Color, int Material, typename Position, typename Range>
struct detector<Color, Material, Conversion, Position, Range>
{
private:
        // typedefs

        typedef detector<Color, Material, Jumps,      Position, Range                   > DoJumps;

        // only pawns can promote and they always have rules::range::distance_1
        typedef detector<Color, Material, Promotions, Position, rules::range::distance_1> DoPromotions;

public:
        bool operator()(Position const& p) const
        {
                // speculate #jumps > #promotions, so that the || is likely to short-circuit
                return DoJumps()(p) || DoPromotions()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
