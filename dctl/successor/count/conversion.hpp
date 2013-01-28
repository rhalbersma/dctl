#pragma once
#include <dctl/successor/count/enumerator_fwd.hpp>
#include <dctl/successor/count/primary.hpp>
#include <dctl/successor/count/both_jumps.hpp>
#include <dctl/successor/count/both_promotions.hpp>
#include <dctl/successor/count/king_jumps.hpp>
// there are no king promotions
#include <dctl/successor/count/pawn_jumps.hpp>
#include <dctl/successor/count/pawn_promotions.hpp>
#include <dctl/successor/select.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Position>
struct enumerator<Color, Material, Conversion, Position>
{
private:
        // typedefs

        typedef enumerator<Color, Material, Jumps,      Position> DoJumps;
        typedef enumerator<Color, Material, Promotions, Position> DoPromotions;

public:
        int operator()(Position const& p) const
        {
                auto num_moves = DoJumps()(p);
                if (!num_moves)
                        num_moves += DoPromotions()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
