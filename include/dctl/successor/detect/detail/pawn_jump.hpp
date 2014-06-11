#pragma once
#include <dctl/successor/detect/detail/primary_fwd.hpp>
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/successor/propagate/jump.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps detection
template<bool Color, class Position, class Range>
struct Detect<Color, pieces::pawn, select::jump, Position, Range>
{
        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::jump, Position>;

        // representation

        State& propagate_;

public:
        // structors

        explicit Detect(State& p)
        :
                propagate_{p}
        {}

        // function call operators

        bool operator()(Set const& active_pawns) const
        {
                return active_pawns.empty() ? false : branch(active_pawns);
        }

private:
        bool branch(Set const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                return branch_dispatch(active_pawns, std::pair<is_backward_pawn_jump_t<Rules>, is_orthogonal_jump_t<Rules>>{});
        }

        // overload for pawns that only jump in the 2 forward diagonal directions
        bool branch_dispatch(Set const& active_pawns, std::pair<std::false_type, std::false_type>) const
        {
                return
                        parallelize<Compass::left_up   >(active_pawns) ||
                        parallelize<Compass::right_up  >(active_pawns)
                ;
        }

        // overload for pawns that jump in the 4 forward and backward diagonal directions
        bool branch_dispatch(Set const& active_pawns, std::pair<std::true_type, std::false_type>) const
        {
                return
                        parallelize<Compass::left_up   >(active_pawns) ||
                        parallelize<Compass::right_up  >(active_pawns) ||
                        parallelize<Compass::left_down >(active_pawns) ||
                        parallelize<Compass::right_down>(active_pawns)
                ;
        }

        // overload for pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        bool branch_dispatch(Set const& active_pawns, std::pair<std::false_type, std::true_type>) const
        {
                return
                        parallelize<Compass::up        >(active_pawns) ||
                        parallelize<Compass::left_up   >(active_pawns) ||
                        parallelize<Compass::right_up  >(active_pawns) ||
                        parallelize<Compass::left      >(active_pawns) ||
                        parallelize<Compass::right     >(active_pawns)
                ;
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        bool branch_dispatch(Set const& active_pawns, std::pair<std::true_type, std::true_type>) const
        {
                return
                        parallelize<Compass::up        >(active_pawns) ||
                        parallelize<Compass::left_up   >(active_pawns) ||
                        parallelize<Compass::right_up  >(active_pawns) ||
                        parallelize<Compass::left      >(active_pawns) ||
                        parallelize<Compass::right     >(active_pawns) ||
                        parallelize<Compass::left_down >(active_pawns) ||
                        parallelize<Compass::right_down>(active_pawns) ||
                        parallelize<Compass::down      >(active_pawns)
                ;
        }

        template<int Direction>
        bool parallelize(Set const& active_pawns) const
        {
                return !Sandwich<Board, Direction, rules::range::distance_1>()(
                        active_pawns, propagate_.template targets_with_pawn<Direction>(), propagate_.path()
                ).empty();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
