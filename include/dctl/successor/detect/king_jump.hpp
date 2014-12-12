#pragma once
#include <dctl/angle.hpp>                               // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/select/jump.hpp>
#include <dctl/successor/tracker.hpp>                   // Tracker

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sandwich

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position>
class Detect<ToMove, IsReverse, PieceKingType, select::jump, Position>
{
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;
        using State = Tracker<ToMove, Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        State const& tracker;

public:
        explicit Detect(State const& t)
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
                return Sandwich<board_type, Direction, is_long_ranged_king_t<rules_type>>{}(
                        active_kings, tracker.template targets<Direction>(), tracker.path()
                ).any();
        }
};

}       // namespace successor
}       // namespace dctl
