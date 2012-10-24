#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/Generator_fwd.hpp>
#include <dctl/successor/generation/Primary.hpp>
#include <dctl/successor/generation/BothJumps.hpp>
#include <dctl/successor/generation/BothMoves.hpp>
#include <dctl/successor/generation/KingJumps.hpp>
#include <dctl/successor/generation/KingMoves.hpp>
#include <dctl/successor/generation/PawnJumps.hpp>
#include <dctl/successor/generation/PawnMoves.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/capture/State.hpp>
#include <dctl/node/Stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Legal, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material, Jumps, Position> DoJumps;
        typedef generator<Color, Material, Moves, Position> DoMoves;
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
                        (DoMoves(moves_))(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
