#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/successor/propagate/jump.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>

#include <dctl/angle/directions.hpp>                    // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t

namespace dctl {
namespace successor {

// partial specialization for pawn jumps detection
template<bool Color, class Position, class Range>
class Detect<Color, pieces::pawn, select::jump, Position, Range>
{
public:
        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

private:
        using Rules = rules_type_t<Position>;
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using State = Propagate<select::jump, Position>;

        static constexpr auto orientation = orientation_v<Board, Color>;

        // representation

        State& propagate_;

public:
        // constructors

        explicit Detect(State& p)
        :
                propagate_{p}
        {}

        // function call operators

        auto operator()(Set const& active_pawns) const
        {
                return active_pawns.any() ? branch(active_pawns) : false;
        }

private:
        auto branch(Set const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                return branch_dispatch(active_pawns, std::pair<is_backward_pawn_jump_t<Rules>, is_orthogonal_jump_t<Rules>>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        auto branch_dispatch(Set const& active_pawns, std::pair<std::false_type, std::false_type>) const
        {
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        parallelize<left_up   (orientation)>(active_pawns) ||
                        parallelize<right_up  (orientation)>(active_pawns)
                ;
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        auto branch_dispatch(Set const& active_pawns, std::pair<std::true_type, std::false_type>) const
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
        auto branch_dispatch(Set const& active_pawns, std::pair<std::false_type, std::true_type>) const
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
        auto branch_dispatch(Set const& active_pawns, std::pair<std::true_type, std::true_type>) const
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
        auto parallelize(Set const& active_pawns) const
        {
                return Sandwich<Board, Direction, std::false_type>{}(
                        active_pawns, propagate_.template targets_with_pawn<Direction>(), propagate_.path()
                ).any();
        }
};

}       // namespace successor
}       // namespace dctl
