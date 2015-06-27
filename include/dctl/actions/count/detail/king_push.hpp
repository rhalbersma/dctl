#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up, left_down, right_down
#include <dctl/player.hpp>                               // Player
#include <dctl/piece.hpp>                               // king
#include <dctl/actions/count/detail/primary_fwd.hpp>  // Count (primary template)
#include <dctl/actions/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sink
#include <xstd/cstddef.hpp>                             // _z

namespace dctl {
namespace actions {
namespace detail {

template<Player ToMove, bool Reverse, class State>
class Count<ToMove, Piece::king, select::push, Reverse, State>
{
        using board_type = board_type_t<State>;
        using rules_type = rules_type_t<State>;
        using   set_type =   set_type_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        State const& position;

public:
        explicit Count(State const& p)
        :
                position{p}
        {}

        auto operator()() const
        {
                return count(position.pieces(ToMove, Piece::king));
        }

        auto operator()(set_type const& active_kings) const
        {
                return count(active_kings);
        }

private:
        auto count(set_type const& active_kings) const
        {
                using namespace xstd::support_literals;

                if (active_kings.none())
                        return 0_z;

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
                return Sink<board_type, Direction, king_range_category_t<rules_type>>{}(
                        active_kings, position.not_occupied()
                ).count();
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
