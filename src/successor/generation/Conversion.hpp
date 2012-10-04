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

// partial specialization for conversion successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Conversion, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material, Jumps,      Position> DoJumps;
        typedef generator<Color, Material, Promotions, Position> DoPromotions;
        typedef capture::State<Position> State;

        // representation

        Stack& moves_;

public:
        // structors

        explicit generator(Stack& m)
        : 
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                State capture_(p, moves_);

                // parentheses around function objects to avoid "C++'s most vexing parse"
                (DoJumps(capture_))(p);
                if (moves_.empty())
                        (DoPromotions(moves_))(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
