#pragma once
#include <type_traits>                  // false_type, true_type
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/successor/jumps.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Jumps, Position>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;
        typedef typename Position::rules_type Rules;
        typedef Propagate<Jumps, Position> State;

public:
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                State capture(p);
                precedence(p, capture, moves);
        }

private:
        void precedence(Position const& p, State& capture, Vector<Move>& moves) const
        {
                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, capture, moves, typename rules::traits<Rules>::is_absolute_king_precedence());
        }

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture, Vector<Move>& moves, std::false_type) const
        {
                KingJumps{capture, moves}(p);
                PawnJumps{capture, moves}(p);
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture, Vector<Move>& moves, std::true_type) const
        {
                KingJumps{capture, moves}(p);
                if (capture.empty())
                        PawnJumps{capture, moves}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
