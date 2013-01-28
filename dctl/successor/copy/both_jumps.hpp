#pragma once
#include <type_traits>                  // false_type, true_type
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/capture/state.hpp>
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
        typedef capture::State<Position> State;

public:
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                State capture_(p, moves);
                precedence(p, capture_);
        }

private:
        void precedence(Position const& p, State& capture_) const
        {
                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, capture_, typename rules::traits<Rules>::is_absolute_king_precedence());
        }

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture_, std::false_type) const
        {
                KingJumps{capture_}(p);
                PawnJumps{capture_}(p);
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture_, std::true_type) const
        {
                KingJumps{capture_}(p);
                if (capture_.empty())
                        PawnJumps{capture_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
