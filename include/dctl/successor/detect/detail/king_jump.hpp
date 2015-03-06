#pragma once
#include <dctl/angle.hpp>                               // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/successor/detail/tracker.hpp>            // Tracker
#include <dctl/successor/detect/detail/primary_fwd.hpp> // Detect (primary template)
#include <dctl/successor/select/jump.hpp>               // jump

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sandwich

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, bool Reverse, class Position>
class Detect<ToMove, Piece::king, select::jump, Reverse, Position>
{
        using   board_type = board_type_t<Position>;
        using   rules_type = rules_type_t<Position>;
        using     set_type =   set_type_t<Position>;
        using tracker_type = detail::Tracker<ToMove, Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        tracker_type const& tracker;

public:
        explicit Detect(tracker_type const& t)
        :
                tracker{t}
        {}

        auto operator()(set_type const& active_kings) const
        {
                return active_kings.none() ? false : branch(active_kings);
        }

private:
        auto branch(set_type const& active_kings) const
        {
                // tag dispatching on king jump directions
                return branch_dispatch(active_kings, is_orthogonal_jump_t<rules_type>{});
        }

        // kings that jump in the 4 diagonal directions
        auto branch_dispatch(set_type const& active_kings, std::false_type) const
        {
                return
                        parallelize<left_up   (orientation)>(active_kings) ||
                        parallelize<right_up  (orientation)>(active_kings) ||
                        parallelize<left_down (orientation)>(active_kings) ||
                        parallelize<right_down(orientation)>(active_kings)
                ;
        }

        // kings that jump in the 8 diagonal and orthogonal directions
        auto branch_dispatch(set_type const& active_kings, std::true_type) const
        {
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

        template<int Direction>
        auto parallelize(set_type const& active_kings) const
        {
                return Sandwich<board_type, Direction, king_range_category<rules_type>>{}(
                        active_kings, tracker.template targets<Direction>(), tracker.path()
                ).any();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
