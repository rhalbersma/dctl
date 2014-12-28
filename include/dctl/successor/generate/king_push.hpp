#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up, left_down, right_down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/ray.hpp>                                 // make_iterator
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t, value_type_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position, class Sequence>
class Generate<ToMove, select::push, IsReverse, PieceKingType, Position, Sequence>
{
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        set_type const& not_occupied;
        Sequence& moves;

public:
        Generate(set_type const& s, Sequence& m)
        :
                not_occupied{s},
                moves{m}
        {}

        auto operator()(set_type const& active_kings) const
        {
                find_dispatch(active_kings, is_long_ranged_king_t<rules_type>{});
        }

private:
        auto find_dispatch(set_type const& active_kings, std::false_type) const
        {
                if (active_kings.none())
                        return;

                transform_movers<left_up   (orientation)>(active_kings);
                transform_movers<right_up  (orientation)>(active_kings);
                transform_movers<left_down (orientation)>(active_kings);
                transform_movers<right_down(orientation)>(active_kings);
        }

        auto find_dispatch(set_type const& active_kings, std::true_type) const
        {
                for (auto&& from_sq : active_kings) {
                        transform_targets(along_ray<left_up   (orientation)>(from_sq));
                        transform_targets(along_ray<right_up  (orientation)>(from_sq));
                        transform_targets(along_ray<left_down (orientation)>(from_sq));
                        transform_targets(along_ray<right_down(orientation)>(from_sq));
                }
        }

        template<int Direction>
        auto transform_movers(set_type const& active_kings) const
        {
                auto const movers = active_kings & set_type(*std::prev(along_wave<Direction>(not_occupied)));
                for (auto&& from_sq : movers)
                        moves.emplace_back(from_sq, *++along_ray<Direction>(from_sq), ToMove);
        }

        template<class Iterator>
        auto transform_targets(Iterator from) const
        {
                auto const targets = ray::classical(from, not_occupied);
                for (auto&& dest_sq : targets)
                        moves.emplace_back(*from, dest_sq, ToMove);
        }

        template<int Direction>
        static auto along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace successor
}       // namespace dctl
