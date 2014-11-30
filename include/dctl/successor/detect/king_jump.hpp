#pragma once
#include <dctl/color.hpp>
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>                         // king
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <dctl/successor/select/jump.hpp>

#include <dctl/angle/directions.hpp>                    // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>

namespace dctl {
namespace successor {

// partial specialization for king jumps detection
template<Color ToMove, class Position, class Range>
class Detect<ToMove, pieces::king, select::jump, Position, Range>
{
        using Rules = rules_type_t<Position>;
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using State = Tracker<ToMove, Position>;

        static constexpr auto orientation = orientation_v<Board, ToMove>;
        State const& propagate;

public:
        // constructors

        explicit Detect(State const& p)
        :
                propagate{p}
        {}

        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

        // function call operators

        auto operator()(Set const& active_kings) const
        {
                return active_kings.none() ? false : branch(active_kings);
        }

private:
        auto branch(Set const& active_kings) const
        {
                // tag dispatching on king jump directions
                return branch_dispatch(active_kings, is_orthogonal_jump_t<Rules>{});
        }

        // kings that jump in the 4 diagonal directions
        auto branch_dispatch(Set const& active_kings, std::false_type) const
        {
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        parallelize<left_up   (orientation)>(active_kings) ||
                        parallelize<right_up  (orientation)>(active_kings) ||
                        parallelize<left_down (orientation)>(active_kings) ||
                        parallelize<right_down(orientation)>(active_kings)
                ;
        }

        // kings that jump in the 8 diagonal and orthogonal directions
        auto branch_dispatch(Set const& active_kings, std::true_type) const
        {
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        parallelize<up        (orientation)>(active_kings) ||
                        parallelize<left_up   (orientation)>(active_kings) ||
                        parallelize<right_up  (orientation)>(active_kings) ||
                        parallelize<left      (orientation)>(active_kings) ||
                        parallelize<right     (orientation)>(active_kings) ||
                        parallelize<left_down (orientation)>(active_kings) ||
                        parallelize<right_down(orientation)>(active_kings) ||
                        parallelize<down      (orientation)>(active_kings)
                ;
        }

        template<int Direction, class Set>
        auto parallelize(Set const& active_kings) const
        {
                return Sandwich<Board, Direction, Range>{}(
                        active_kings, propagate.template targets<Direction>(), propagate.path()
                ).any();
        }
};

}       // namespace successor
}       // namespace dctl
