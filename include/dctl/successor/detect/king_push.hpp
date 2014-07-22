#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>                         // king
#include <dctl/successor/propagate/push.hpp>            // Propagate (push specialization)
#include <dctl/successor/select/push.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/type_traits.hpp>         // board_type_t, rules_type_t
#include <type_traits>

namespace dctl {
namespace successor {

// partial specialization for king moves detection
template<bool Color, class Position, class Range>
class Detect<Color, pieces::king, select::push, Position, Range>
{
public:
        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

private:
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using State = Propagate<select::push, Position>;

        static constexpr auto orientation = orientation_v<Board, Color>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit Detect(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        auto operator()(Set const& active_kings) const
        {
                return active_kings.empty() ? false : branch(active_kings);
        }

private:
        auto branch(Set const& active_kings) const
        {
                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                return
                        parallelize<left_up   (orientation)>(active_kings) ||
                        parallelize<right_up  (orientation)>(active_kings) ||
                        parallelize<left_down (orientation)>(active_kings) ||
                        parallelize<right_down(orientation)>(active_kings)
                ;
        }

        template<int Direction>
        auto parallelize(Set const& active_kings) const
        {
                return !Sink<Board, Direction, std::false_type>{}(
                        active_kings, propagate_.path()
                ).empty();
        }
};

}       // namespace successor
}       // namespace dctl
