#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../capture/State.hpp"
#include "../../node/Stack.hpp"

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
