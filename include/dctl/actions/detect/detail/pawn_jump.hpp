#pragma once
#include <dctl/angle.hpp>                               // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/player.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/actions/detail/filter.hpp>             // Precedence, Unique
#include <dctl/actions/detail/raii.hpp>               // ToggleKingTargets
#include <dctl/actions/detail/tracker.hpp>            // Tracker
#include <dctl/actions/detect/detail/primary_fwd.hpp> // Detect (primary template)
#include <dctl/actions/select/jump.hpp>               // jump

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/rule_traits.hpp>                         // is_backward_pawn_jump, is_orthogonal_jump, is_pawn_jump_king
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sandwich

namespace dctl {
namespace actions {
namespace detail {

template<Player ToMove, bool Reverse, class State>
class Detect<ToMove, Piece::pawn, select::jump, Reverse, State>
{
        using   board_type = board_type_t<State>;
        using   rules_type = rules_type_t<State>;
        using     set_type =   set_type_t<State>;
        using tracker_type = detail::Tracker<ToMove, State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        tracker_type& tracker;

public:
        explicit Detect(tracker_type& t)
        :
                tracker{t}
        {}

        auto operator()(set_type const& active_pawns) const
        {
                return active_pawns.any() ? king_targets_dispatch(active_pawns, is_pawn_jump_king_t<rules_type>{}) : false;
        }

private:
        // pawns that can capture kings
        auto king_targets_dispatch(set_type const& active_pawns, std::true_type) const
        {
                return branch(active_pawns);
        }

        // pawns that cannot capture kings
        auto king_targets_dispatch(set_type const& active_pawns, std::false_type) const
        {
                raii::ToggleKingTargets<tracker_type> guard{tracker};
                return branch(active_pawns);
        }

        auto branch(set_type const& active_pawns) const
        {
                return branch_dispatch(active_pawns, is_backward_pawn_jump_t<rules_type>{}, is_orthogonal_jump_t<rules_type>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        auto branch_dispatch(set_type const& active_pawns, std::false_type, std::false_type) const
        {
                return
                        parallelize<left_up   (orientation)>(active_pawns) ||
                        parallelize<right_up  (orientation)>(active_pawns)
                ;
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        auto branch_dispatch(set_type const& active_pawns, std::true_type, std::false_type) const
        {
                return
                        parallelize<left_up   (orientation)>(active_pawns) ||
                        parallelize<right_up  (orientation)>(active_pawns) ||
                        parallelize<left_down (orientation)>(active_pawns) ||
                        parallelize<right_down(orientation)>(active_pawns)
                ;
        }

        // pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        auto branch_dispatch(set_type const& active_pawns, std::false_type, std::true_type) const
        {
                return
                        parallelize<up        (orientation)>(active_pawns) ||
                        parallelize<left_up   (orientation)>(active_pawns) ||
                        parallelize<right_up  (orientation)>(active_pawns) ||
                        parallelize<left      (orientation)>(active_pawns) ||
                        parallelize<right     (orientation)>(active_pawns)
                ;
        }

        // pawns that jump in the 8 diagonal and orthogonal directions
        auto branch_dispatch(set_type const& active_pawns, std::true_type, std::true_type) const
        {
                return
                        parallelize<up        (orientation)>(active_pawns) ||
                        parallelize<left_up   (orientation)>(active_pawns) ||
                        parallelize<right_up  (orientation)>(active_pawns) ||
                        parallelize<left      (orientation)>(active_pawns) ||
                        parallelize<right     (orientation)>(active_pawns) ||
                        parallelize<left_down (orientation)>(active_pawns) ||
                        parallelize<right_down(orientation)>(active_pawns) ||
                        parallelize<down      (orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(set_type const& active_pawns) const
        {
                return Sandwich<board_type, Direction, short_ranged_tag>{}(
                        active_pawns, tracker.template targets<Direction>(), tracker.path()
                ).any();
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
