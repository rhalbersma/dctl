#pragma once
#include <functional>                   // function
#include "Generator_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothPromotions.hpp"
#include "KingJumps.hpp"
// there are no king promotions
#include "PawnJumps.hpp"
#include "PawnPromotions.hpp"
#include "../Select.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Conversion, Position>
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef generator<Color, Material, Jumps, Position> DoJumps;
        typedef generator<Color, Material, Promotions, Position> DoPromotions;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                DoJumps()(p, moves);
                if (moves.empty())
                        DoPromotions()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
