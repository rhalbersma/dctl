#pragma once
#include <functional>                   // function
#include "Enumerator_fwd.hpp"
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

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Position>
struct enumerator<Color, Material, Legal, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef enumerator<Color, Material, Jumps, Position> DoJumps;
        typedef enumerator<Color, Material, Moves, Position> DoMoves;
        typedef capture::State<Position> State;

        // representation

        Stack mutable moves_;

public:
        // structors

        enumerator()
        :
                moves_()
        {
                moves_.reserve(MOVE_RESERVE);
        }

        // function call operators

        int operator()(Position const& p) const
        {
                State capture_(p, moves_);
                
                // parentheses around function objects to avoid "C++'s most vexing parse"
                auto num_moves = (DoJumps(capture_))(p);
                if (!num_moves)
                        num_moves += DoMoves()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
