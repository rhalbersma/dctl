#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/propagate/push.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/pawn.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/type_traits.hpp>         // board_type_t, rules_type_t

namespace dctl {
namespace successor {

// partial specialization for pawn moves enumeration
template<bool Color, class Position>
class Count<Color, pieces::pawn, select::push, Position>
{
public:
        // enforce reference semantics
        Count(Count const&) = delete;
        Count& operator=(Count const&) = delete;

private:
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using State = Propagate<select::push, Position>;

        static constexpr auto orientation = orientation_v<Board, Color>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit Count(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        auto operator()(Set const& active_pawns) const
        {
                return active_pawns.empty() ? 0 : branch(active_pawns);
        }

private:
        auto branch(Set const& active_pawns) const
        {
                return
                        parallelize<left_up (orientation)>(active_pawns) +
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(Set const& active_pawns) const
        {
                return Sink<Board, Direction, rules::range::distance_1>{}(
                        active_pawns, propagate_.path()
                ).size();
        }
};

}       // namespace successor
}       // namespace dctl
