#pragma once
#include <dctl/successor/count/detail/primary_fwd.hpp>
#include <dctl/successor/propagate/push.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/pawn.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
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
        using Set = typename Board::set_type;
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

        int operator()(Set const& active_pawns) const
        {
                return active_pawns.empty() ? 0 : branch(active_pawns);
        }

private:
        int branch(Set const& active_pawns) const
        {
                return
                        parallelize<left_up (orientation)>(active_pawns) +
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        int parallelize(Set const& active_pawns) const
        {
                return Sink<Board, Direction, rules::range::distance_1>{}(
                        active_pawns, propagate_.path()
                ).size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
