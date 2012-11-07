#pragma once
#include <functional>                   // function
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/primary.hpp>
#include <dctl/successor/generation/both_jumps.hpp>
#include <dctl/successor/generation/both_promotions.hpp>
#include <dctl/successor/generation/king_jumps.hpp>
// there are no king promotions
#include <dctl/successor/generation/pawn_jumps.hpp>
#include <dctl/successor/generation/pawn_promotions.hpp>
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
                        DoPromotions{moves_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
