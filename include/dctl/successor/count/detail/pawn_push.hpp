#pragma once
#include <dctl/successor/count/detail/primary_fwd.hpp>
#include <dctl/successor/propagate/push.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/pawn.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves enumeration
template<bool Color, class Position>
struct Count<Color, pieces::pawn, select::push, Position>
{
        // enforce reference semantics
        Count(Count const&) = delete;
        Count& operator=(Count const&) = delete;

private:
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::push, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit Count(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        template<class Set>
        int operator()(Set const& active_pawns) const
        {
                return active_pawns.empty() ? 0 : branch(active_pawns);
        }

private:
        template<class Set>
        int branch(Set const& active_pawns) const
        {
                return
                        parallelize<Compass::left_up >(active_pawns) +
                        parallelize<Compass::right_up>(active_pawns)
                ;
        }

        template<int Direction, class Set>
        int parallelize(Set const& active_pawns) const
        {
                return Sink<Board, Direction, rules::range::distance_1>()(
                        active_pawns, propagate_.path()
                ).size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
