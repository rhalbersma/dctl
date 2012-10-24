#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/enumeration/Primary.hpp>
#include <dctl/successor/enumeration/BothJumps.hpp>
#include <dctl/successor/enumeration/BothPromotions.hpp>
#include <dctl/successor/enumeration/KingJumps.hpp>
// there are no king promotions
#include <dctl/successor/enumeration/PawnJumps.hpp>
#include <dctl/successor/enumeration/PawnPromotions.hpp>
#include <dctl/successor/Select.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Position>
struct enumerator<Color, Material, Conversion, Position>
:
        public std::function<int(Position const&)>
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
