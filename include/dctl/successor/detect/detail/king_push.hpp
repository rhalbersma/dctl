#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up, left_down, right_down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/successor/detect/detail/primary_fwd.hpp> // Detect (primary template)
#include <dctl/successor/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/type_traits.hpp>                         // board_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sink
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, bool Reverse, class Position>
class Detect<ToMove, Piece::king, select::push, Reverse, Position>
{
        using board_type = board_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        Position const& position;

public:
        explicit Detect(Position const& p)
        :
                position{p}
        {}

        auto operator()() const
        {
                return detect(position.pieces(ToMove, Piece::king));
        }

        auto operator()(set_type const& active_kings) const
        {
                return detect(active_kings);
        }

private:
        auto detect(set_type const& active_kings) const
        {
                if (active_kings.none())
                        return false;

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
                return Sink<board_type, Direction, short_ranged_tag>{}(
                        active_kings, position.not_occupied()
                ).any();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
