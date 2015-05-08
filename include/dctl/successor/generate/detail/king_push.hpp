#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up, left_down, right_down
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/successor/generate/detail/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/ray.hpp>                                 // make_iterator
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t, value_type_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, bool Reverse, class Position, class Sequence>
class Generate<ToMove, Piece::king, select::push, Reverse, Position, Sequence>
{
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        Position const& position;
        Sequence& moves;

public:
        Generate(Position const& p, Sequence& m)
        :
                position{p},
                moves{m}
        {}

        auto operator()() const
        {
                generate(position.pieces(ToMove, Piece::king));
        }

        auto operator()(set_type const& active_kings) const
        {
                generate(active_kings);
        }

private:
        auto generate(set_type const& active_kings) const
        {
                generate_dispatch(active_kings, king_range_category_t<rules_type>{});
        }

        auto generate_dispatch(set_type const& active_kings, short_ranged_tag) const
        {
                if (active_kings.none())
                        return;

                generate_movers<left_up   (orientation)>(active_kings);
                generate_movers<right_up  (orientation)>(active_kings);
                generate_movers<left_down (orientation)>(active_kings);
                generate_movers<right_down(orientation)>(active_kings);
        }

        auto generate_dispatch(set_type const& active_kings, long_ranged_tag) const
        {
                for (auto from_sq : active_kings) {
                        generate_targets(along_ray<left_up   (orientation)>(from_sq));
                        generate_targets(along_ray<right_up  (orientation)>(from_sq));
                        generate_targets(along_ray<left_down (orientation)>(from_sq));
                        generate_targets(along_ray<right_down(orientation)>(from_sq));
                }
        }

        template<int Direction>
        auto generate_movers(set_type const& active_kings) const
        {
                auto const movers = active_kings & set_type(*std::prev(along_wave<Direction>(position.not_occupied())));
                for (auto from_sq : movers)
                        moves.emplace_back(from_sq, *++along_ray<Direction>(from_sq), ToMove);
        }

        template<class Iterator>
        auto generate_targets(Iterator from) const
        {
                auto const targets = ray::classical(from, position.not_occupied());
                for (auto dest_sq : targets)
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

}       // namespace detail
}       // namespace successor
}       // namespace dctl
