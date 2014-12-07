#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PiecePawnType
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/select/push.hpp>

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/type_traits.hpp>                         // board_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sink
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position>
class Count<ToMove, IsReverse, PiecePawnType, select::push, Position>
{
        using board_type = board_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        set_type const& not_occupied;

public:
        explicit Count(set_type const& s)
        :
                not_occupied{s}
        {}

        auto operator()(set_type const& active_pawns) const
        {
                return active_pawns.any() ? branch(active_pawns) : 0;
        }

private:
        auto branch(set_type const& active_pawns) const
        {
                return
                        parallelize<left_up (orientation)>(active_pawns) +
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(set_type const& active_pawns) const
        {
                return Sink<board_type, Direction, std::false_type>{}(
                        active_pawns, not_occupied
                ).count();
        }
};

}       // namespace successor
}       // namespace dctl
