#pragma once
#include <functional>                   // function
#include <dctl/successor/generation/Generator_fwd.hpp>
#include <dctl/successor/generation/Primary.hpp>
#include <dctl/successor/generation/BothJumps.hpp>
#include <dctl/successor/generation/BothPromotions.hpp>
#include <dctl/successor/generation/KingJumps.hpp>
// there are no king promotions
#include <dctl/successor/generation/PawnJumps.hpp>
#include <dctl/successor/generation/PawnPromotions.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/node/Stack.hpp>

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
