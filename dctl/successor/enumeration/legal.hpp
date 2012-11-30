#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/enumeration/primary.hpp>
#include <dctl/successor/enumeration/both_jumps.hpp>
#include <dctl/successor/enumeration/both_moves.hpp>
#include <dctl/successor/enumeration/king_jumps.hpp>
#include <dctl/successor/enumeration/king_moves.hpp>
#include <dctl/successor/enumeration/pawn_jumps.hpp>
#include <dctl/successor/enumeration/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/capture/state.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Position>
struct enumerator<Color, Material, Legal, Position>
{
private:
        // typedefs

        typedef enumerator<Color, Material, Jumps, Position> DoJumps;
        typedef enumerator<Color, Material, Moves, Position> DoMoves;
        typedef capture::State<Position> State;

        // representation

        MoveArena mar_;
        MoveAlloc mal_;
        Stack mutable moves_;

public:
        // structors

        enumerator()
        :
                mar_(),
                mal_(mar_),
                moves_(mal_)
        {
                moves_.reserve(DCTL_PP_VECTOR_RESERVE);
        }

        // function call operators

        int operator()(Position const& p) const
        {
                State capture_ { p, moves_ };

                auto num_moves = (DoJumps(capture_))(p);
                if (!num_moves)
                        num_moves += DoMoves()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
