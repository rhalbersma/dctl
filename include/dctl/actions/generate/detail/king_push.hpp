#pragma once
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // king
#include <dctl/actions/generate/detail/primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/push.hpp>                 // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                           // make_iterator
#include <dctl/rule_traits.hpp>                         // is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t, value_t
#include <cstddef>                                      // size_t
#include <iterator>                                     // prev

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, bool Reverse, class State, class Sequence>
class Generate<ToMove, Piece::king, select::push, Reverse, State, Sequence>
{
        using board_type = board_t<State>;
        using rules_type = rules_t<State>;
        using   set_type =   set_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        State const& state;
        Sequence& moves;

public:
        Generate(State const& s, Sequence& m)
        :
                state{s},
                moves{m}
        {}

        auto operator()() const
        {
                generate(state.pieces(ToMove, Piece::king));
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
                active_kings.for_each([&](auto const& from_sq){
                        generate_targets(along_ray<left_up   (orientation)>(from_sq));
                        generate_targets(along_ray<right_up  (orientation)>(from_sq));
                        generate_targets(along_ray<left_down (orientation)>(from_sq));
                        generate_targets(along_ray<right_down(orientation)>(from_sq));
                });
        }

        template<int Direction>
        auto generate_movers(set_type const& active_kings) const
        {
                auto const movers = active_kings & set_type(*std::prev(along_wave<Direction>(state.not_occupied())));
                movers.for_each([&](auto const& from_sq){
                        moves.emplace_back(from_sq, *++along_ray<Direction>(from_sq), ToMove);
                });
        }

        template<class Iterator>
        auto generate_targets(Iterator from) const
        {
                auto const targets = ray::classical(from, state.not_occupied());
                targets.for_each([&](auto const& dest_sq){
                        moves.emplace_back(*from, dest_sq, ToMove);
                });
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
}       // namespace actions
}       // namespace dctl
