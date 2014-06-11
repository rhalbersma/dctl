#pragma once
#include <dctl/successor/detect/detail/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>                         // king
#include <dctl/successor/propagate/push.hpp>            // Propagate (push specialization)
#include <dctl/successor/select/push.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves detection
template<bool Color, class Position, class Range>
struct Detect<Color, pieces::king, select::push, Position, Range>
{
        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

private:
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::push, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit Detect(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        bool operator()(Set const& active_kings) const
        {
                return active_kings.empty() ? false : branch(active_kings);
        }

private:
        bool branch(Set const& active_kings) const
        {
                return
                        parallelize<Compass::left_down >(active_kings) ||
                        parallelize<Compass::right_down>(active_kings) ||
                        parallelize<Compass::left_up   >(active_kings) ||
                        parallelize<Compass::right_up  >(active_kings)
                ;
        }

        template<int Direction>
        bool parallelize(Set const& active_kings) const
        {
                return !Sink<Board, Direction, rules::range::distance_1>()(
                        active_kings, propagate_.path()
                ).empty();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
