#pragma once
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>             // king
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/patterns.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king moves detection
template<bool Color, class Position, class Range>
struct detect<Color, pieces::king, select::moves, Position, Range>
{


private:
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::moves, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit detect(State const& p)
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
                return (
                        parallelize< Compass::left_down  >(active_kings) ||
                        parallelize< Compass::right_down >(active_kings) ||
                        parallelize< Compass::left_up    >(active_kings) ||
                        parallelize< Compass::right_up   >(active_kings)
                );
        }

        template<int Direction, class Set>
        bool parallelize(Set const& active_kings) const
        {
                return !Sink<Board, Direction, rules::range::distance_1>()(
                        active_kings, propagate_.path()
                ).empty();
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
