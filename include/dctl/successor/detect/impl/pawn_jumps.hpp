#pragma once
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/pieces/pawn.hpp>             // pawn
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn jumps detection
template<bool Color, class Position, class Range>
struct detect<Color, pieces::pawn, select::jumps, Position, Range>
{
private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::jumps, Position>;

        // representation

        State& propagate_;

public:
        // structors

        explicit detect(State& p)
        :
                propagate_{p}
        {}

        // function call operators

        template<class Set>
        bool operator()(Set const& active_pawns) const
        {
                return active_pawns.empty() ? false : branch(active_pawns);
        }

private:
        template<class Set>
        bool branch(Set const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                return branch_dispatch(active_pawns, rules::directions::pawn_jump<Rules>{});
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_pawns, rules::directions::all) const
        {
                return
                        branch_dispatch(active_pawns, rules::directions::diag()) ||
                        branch_dispatch(active_pawns, rules::directions::orth())
                ;
        }

        // overload for pawns that jump in the 4 diagonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_pawns, rules::directions::diag) const
        {
                return
                        branch_dispatch(active_pawns, rules::directions::up  ()) ||
                        branch_dispatch(active_pawns, rules::directions::down())
                ;
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_pawns, rules::directions::up) const
        {
                return
                        parallelize<Compass::left_up >(active_pawns) ||
                        parallelize<Compass::right_up>(active_pawns)
                ;
        }

        // overload for pawns that jump in the 2 backward diagonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_pawns, rules::directions::down) const
        {
                return
                        parallelize<Compass::left_down >(active_pawns) ||
                        parallelize<Compass::right_down>(active_pawns)
                ;
        }

        // overload for pawns that jump in the 4 orthogonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_pawns, rules::directions::orth) const
        {
                return
                        parallelize<Compass::left >(active_pawns) ||
                        parallelize<Compass::right>(active_pawns) ||
                        parallelize<Compass::up   >(active_pawns) ||
                        parallelize<Compass::down >(active_pawns)
                ;
        }

        template<int Direction, class Set>
        bool parallelize(Set const& active_pawns) const
        {
                return !Sandwich<Board, Direction, rules::range::distance_1>()(
                        active_pawns, propagate_.template targets_with_pawn<Direction>(), propagate_.path()
                ).empty();
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
