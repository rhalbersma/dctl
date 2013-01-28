#pragma once
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/primary.hpp>
#include <dctl/successor/copy/both_jumps.hpp>
#include <dctl/successor/copy/both_promotions.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
// there are no king promotions
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/successor/copy/pawn_promotions.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversion successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Conversion, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<Color, Material, Jumps,      Position> DoJumps;
        typedef generator<Color, Material, Promotions, Position> DoPromotions;
        typedef capture::State<Position> State;

        // representation

        Vector<Move>& moves_;

public:
        // structors

        /*explicit*/ generator(Vector<Move>& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                State capture_ { p, moves_ };

                DoJumps{capture_}(p);
                if (moves_.empty())
                        DoPromotions{moves_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
