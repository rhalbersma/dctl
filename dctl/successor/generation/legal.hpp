#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/primary.hpp>
#include <dctl/successor/generation/both_jumps.hpp>
#include <dctl/successor/generation/both_moves.hpp>
#include <dctl/successor/generation/king_jumps.hpp>
#include <dctl/successor/generation/king_moves.hpp>
#include <dctl/successor/generation/pawn_jumps.hpp>
#include <dctl/successor/generation/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/capture/state.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Legal, Position>
:
        // enforce reference semantics
        private boost::noncopyable
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

        /*explicit*/ generator(Stack& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                State capture_ { p, moves_ };

                DoJumps{capture_}(p);
                if (moves_.empty())
                        DoMoves{moves_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
