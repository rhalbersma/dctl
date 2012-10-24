#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/enumeration/Primary.hpp>
#include <dctl/successor/enumeration/BothJumps.hpp>
#include <dctl/successor/enumeration/BothMoves.hpp>
#include <dctl/successor/enumeration/KingJumps.hpp>
#include <dctl/successor/enumeration/KingMoves.hpp>
#include <dctl/successor/enumeration/PawnJumps.hpp>
#include <dctl/successor/enumeration/PawnMoves.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/capture/State.hpp>
#include <dctl/node/Stack.hpp>

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
