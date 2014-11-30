#pragma once
#include <dctl/color.hpp>
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>                         // king
#include <dctl/successor/select/push.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up, left_down, right_down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t
#include <type_traits>

namespace dctl {
namespace successor {

// partial specialization for king moves detection
template<Color ToMove, class Position, class Range>
class Detect<ToMove, pieces::king, select::push, Position, Range>
{
public:
        // enforce reference semantics
        Detect(Detect const&) = delete;
        Detect& operator=(Detect const&) = delete;

private:
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;

        static constexpr auto orientation = orientation_v<Board, ToMove>;

        // representation

        Set const& propagate;

public:
        // constructors

        explicit Detect(Set const& p)
        :
                propagate{p}
        {}

        // function call operators

        auto operator()(Set const& active_kings) const
        {
                return active_kings.none() ? false : branch(active_kings);
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
                return Sink<Board, Direction, std::false_type>{}(
                        active_kings, propagate
                ).any();
        }
};

}       // namespace successor
}       // namespace dctl
