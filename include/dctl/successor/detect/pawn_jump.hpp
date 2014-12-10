#pragma once
#include <dctl/angle.hpp>                               // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PiecePawnType
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/select/jump.hpp>
#include <dctl/successor/tracker.hpp>                   // Tracker

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/rule_traits.hpp>                         // is_pawn_jump_king_t, is_backward_pawn_jump_t, is_orthogonal_jump_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sandwich

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position, class Range>
class Detect<ToMove, IsReverse, PiecePawnType, select::jump, Position, Range>
{
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;
        using State = Tracker<ToMove, Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        State& tracker;

public:
        explicit Detect(State& p)
        :
                tracker{p}
        {}

        auto operator()(set_type const& active_pawns) const
        {
                // tag dispatching on whether pawns can capture kings
                // SPECULATE: pawns are present
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
                tracker.toggle_king_targets();
                auto const found = branch(active_pawns);
                tracker.toggle_king_targets();
                return found;
        }

        auto branch(set_type const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                return branch_dispatch(active_pawns, is_backward_pawn_jump_t<rules_type>{}, is_orthogonal_jump_t<rules_type>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        auto branch_dispatch(set_type const& active_pawns, std::false_type, std::false_type) const
        {
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        parallelize<left_up   (orientation)>(active_pawns) ||
                        parallelize<right_up  (orientation)>(active_pawns)
                ;
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        auto branch_dispatch(set_type const& active_pawns, std::true_type, std::false_type) const
        {
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
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
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
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
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
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
                return Sandwich<board_type, Direction, std::false_type>{}(
                        active_pawns, tracker.template targets<Direction>(), tracker.path()
                ).any();
        }
};

}       // namespace successor
}       // namespace dctl
