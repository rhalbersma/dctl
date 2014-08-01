#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/propagate/push.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/king.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t

namespace dctl {
namespace successor {

// partial specialization for king moves enumeration
template<bool Color, class Position>
class Count<Color, pieces::king, select::push, Position>
{
public:
        // enforce reference semantics
        Count(Count const&) = delete;
        Count& operator=(Count const&) = delete;

private:
        using Rules = rules_type_t<Position>;
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using State = Propagate<select::push, Position>;

        static constexpr auto orientation = orientation_v<Board, Color>;

        // representation

        State const& propagate_;

public:
        // constructors

        explicit Count(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        auto operator()(Set const& active_kings) const
        {
                return active_kings.empty() ? 0 : branch(active_kings);
        }

private:
        auto branch(Set const& active_kings) const
        {
                return
                        parallelize<left_up   (orientation)>(active_kings) +
                        parallelize<right_up  (orientation)>(active_kings) +
                        parallelize<left_down (orientation)>(active_kings) +
                        parallelize<right_down(orientation)>(active_kings)
                ;
        }

        template<int Direction>
        auto parallelize(Set const& active_kings) const
        {
                return Sink<Board, Direction, is_long_ranged_king_t<Rules>>{}(
                        active_kings, propagate_.path()
                ).size();
        }
};

}       // namespace successor
}       // namespace dctl
