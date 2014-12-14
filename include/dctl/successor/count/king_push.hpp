#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up, left_down, right_down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sink

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position>
class Count<ToMove, select::push, IsReverse, PieceKingType, Position>
{
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        set_type const& not_occupied;

public:
        explicit Count(set_type const& s)
        :
                not_occupied{s}
        {}

        auto operator()(set_type const& active_kings) const
        {
                return active_kings.none() ? 0 : branch(active_kings);
        }

private:
        auto branch(set_type const& active_kings) const
        {
                return
                        parallelize<left_up   (orientation)>(active_kings) +
                        parallelize<right_up  (orientation)>(active_kings) +
                        parallelize<left_down (orientation)>(active_kings) +
                        parallelize<right_down(orientation)>(active_kings)
                ;
        }

        template<int Direction>
        auto parallelize(set_type const& active_kings) const
        {
                return Sink<board_type, Direction, is_long_ranged_king_t<rules_type>>{}(
                        active_kings, not_occupied
                ).count();
        }
};

}       // namespace successor
}       // namespace dctl
