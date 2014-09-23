#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/all_jump.hpp>           // Detect (piece jump specialization)
#include <dctl/successor/detect/all_push.hpp>           // Detect (piece push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<bool Color, class Pieces, class Range>
class Detect<Color, Pieces, select::legal, Range>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using ShortPush = Detect<Color, Pieces, select::push, std::false_type>;
                using ShortJump = Detect<Color, Pieces, select::jump, std::false_type>;

                // speculate #moves > #jumps, so that the logical OR is more likely to short-circuit
                return ShortPush{}(p) || ShortJump{}(p);
        }
};

}       // namespace successor
}       // namespace dctl
