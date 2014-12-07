#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up, left_down, right_down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/select/push.hpp>

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/type_traits.hpp>                         // board_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sink
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position>
class Detect<ToMove, IsReverse, PieceKingType, select::push, Position>
{
        using board_type = board_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        set_type const& not_occupied;

public:
        explicit Detect(set_type const& p)
        :
                not_occupied{p}
        {}

        auto operator()(set_type const& active_kings) const
        {
                // SPECULATE: no kings are present
                return active_kings.none() ? false : branch(active_kings);
        }

private:
        auto branch(set_type const& active_kings) const
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
        auto parallelize(set_type const& active_kings) const
        {
                return Sink<board_type, Direction, std::false_type>{}(
                        active_kings, not_occupied
                ).any();
        }
};

}       // namespace successor
}       // namespace dctl
