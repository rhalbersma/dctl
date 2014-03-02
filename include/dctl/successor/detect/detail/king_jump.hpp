#pragma once
#include <dctl/successor/detect/detail/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>                         // king
#include <dctl/successor/propagate/jump.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps detection
template<bool Color, class Position, class Range>
struct Detect<Color, pieces::king, select::jump, Position, Range>
{
        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::jump, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit Detect(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        template<class Set>
        bool operator()(Set const& active_kings) const
        {
                return active_kings.empty() ? false : branch(active_kings);
        }

private:
        template<class Set>
        bool branch(Set const& active_kings) const
        {
                // tag dispatching on king jump directions
                return branch_dispatch(active_kings, rules::directions::king_jump<Rules>{});
        }

        // overload for kings that jump in the 8 diagonal and orthogonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_kings, rules::directions::all) const
        {
                return
                        branch_dispatch(active_kings, rules::directions::diag()) ||
                        branch_dispatch(active_kings, rules::directions::orth())
                ;
        }

        // overload for kings that jump in the 4 diagonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_kings, rules::directions::diag) const
        {
                return
                        parallelize<Compass::left_up   >(active_kings) ||
                        parallelize<Compass::right_up  >(active_kings) ||
                        parallelize<Compass::left_down >(active_kings) ||
                        parallelize<Compass::right_down>(active_kings)
                ;
        }

        // overload for kings that jump in the 4 orthogonal directions
        template<class Set>
        bool branch_dispatch(Set const& active_kings, rules::directions::orth) const
        {
                return
                        parallelize<Compass::left >(active_kings) ||
                        parallelize<Compass::right>(active_kings) ||
                        parallelize<Compass::up   >(active_kings) ||
                        parallelize<Compass::down >(active_kings)
                ;
        }

        template<int Direction, class Set>
        bool parallelize(Set const& active_kings) const
        {
                return !Sandwich<Board, Direction, Range>{}(
                        active_kings, propagate_.template targets_with_king<Direction>(), propagate_.path()
                ).empty();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
