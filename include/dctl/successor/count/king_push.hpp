#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/propagate/push.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/king.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {

// partial specialization for king moves enumeration
template<bool Color, class Position>
struct Count<Color, pieces::king, select::push, Position>
{
        // enforce reference semantics
        Count(Count const&) = delete;
        Count& operator=(Count const&) = delete;

private:
        using Rules = typename Position::rules_type;
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

        int operator()(Set const& active_kings) const
        {
                return active_kings.empty() ? 0 : branch(active_kings);
        }

private:
        int branch(Set const& active_kings) const
        {
                return
                        parallelize<left_up   (orientation)>(active_kings) +
                        parallelize<right_up  (orientation)>(active_kings) +
                        parallelize<left_down (orientation)>(active_kings) +
                        parallelize<right_down(orientation)>(active_kings)
                ;
        }

        template<int Direction>
        int parallelize(Set const& active_kings) const
        {
                return Sink<Board, Direction, typename rules::range::move<Rules>::type>{}(
                        active_kings, propagate_.path()
                ).size();
        }
};

}       // namespace successor
}       // namespace dctl
