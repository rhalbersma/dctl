#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, Setking_jump
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down, _deg, rotate, inverse
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror
#include <dctl/color.hpp>                               // color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                        // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/utility/type_traits.hpp>                 // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev

namespace dctl {
namespace core {
namespace detail {

template<color ToMove, class Reverse, class State, class Builder>
class generate<ToMove, piece::king, select::jump, Reverse, State, Builder>
{
        using action_type = action_t<Builder>;
        using  board_type =  board_t<Builder>;
        using  rules_type =  rules_t<Builder>;
        using    set_type =    set_t<Builder>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(board::ray::direction_v<Iterator>, inverse(bearing));

        Builder& builder;
public:
        explicit generate(Builder& b) noexcept
        :
                builder{b}
        {}

        auto operator()() const
        {
                raii::Setking_jump<Builder> guard{builder};
                sources();
        }

        template<class Iterator>
        auto try_next(Iterator jumper, passing_promotion_tag) const
        {
                static_assert(is_passing_promotion_or_v<rules_type>);
                assert(builder.is_with(piece::pawn) && builder.is_into(piece::king));
                try_next(jumper);
        }
private:
        auto sources() const
        {
                builder.active_kings().for_each([this](auto const from_sq) {
                        raii::launch<Builder> guard{builder, from_sq};
                        this->source_dispatch(from_sq, jump_category_t<rules_type>{});
                });
        }

        auto source_dispatch(std::size_t from_sq, diagonal_jump_tag) const
        {
                directions_lfold<left_up, right_up, left_down, right_down>(from_sq);
        }

        auto source_dispatch(std::size_t from_sq, orthogonal_jump_tag) const
        {
                directions_lfold<up, left_up, right_up, left, right, left_down, right_down, down>(from_sq);
        }

        template<template<int> class... Directions>
        auto directions_lfold(std::size_t from_sq) const
        {
                (... , first_target(along_ray<Directions<bearing.degrees>{}>(from_sq)));
        }

        template<class Iterator>
        auto first_target(Iterator jumper) const
        {
                slide(jumper, builder.template path<board::ray::direction_v<Iterator>.degrees>());
                if (is_onboard(jumper) && builder.current_targets(jumper)) {
                        assert(is_onboard(std::next(jumper)));
                        capture(jumper);
                }
        }

        template<class Iterator>
        auto capture(Iterator jumper) const
                -> void
        {
                assert(is_onboard(jumper));
                raii::capture<Builder> guard{builder, *jumper};
                land(std::next(jumper));
        }

        template<class Iterator>
        auto land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                try_next(jumper);
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!next_target(jumper))
                        add(jumper);
        }

        template<class Iterator>
        auto next_target(Iterator jumper) const
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                return reverse_dispatch(jumper, king_jump_category_t<rules_type>{});
        }

        template<class Iterator>
        auto reverse_dispatch(Iterator jumper, forward_king_jump_tag) const
        {
                return scan_turn(jumper);
        }

        template<class Iterator>
        auto reverse_dispatch(Iterator jumper, reverse_king_jump_tag) const
        {
                return scan_turn(jumper) | reverse(jumper);
        }

        template<class Iterator>
        auto reverse(Iterator jumper) const
        {
                static_assert(is_reverse_king_jump_v<rules_type>);
                return scan(board::ray::rotate<180>(jumper));
        }

        template<class Iterator>
        auto scan_turn(Iterator jumper) const
        {
                return scan_turn_dispatch(jumper, king_range_category_land_behind_piece_t<rules_type>{});
        }

        template<class Iterator>
        auto scan_turn_dispatch(Iterator jumper, short_ranged_tag) const
        {
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        auto scan_turn_dispatch(Iterator jumper, long_ranged_tag) const
        {
                // builder.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                assert(is_onboard(jumper) && builder.not_occupied(*jumper));
                auto found_next = turn(jumper);
                auto slider = std::next(jumper);
                while (is_onboard(slider) && builder.not_occupied(*slider)) {
                        found_next |= turn(slider);
                        ++slider;
                }
                return found_next |= is_en_prise(slider);
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                return turn_dispatch(jumper, jump_category_t<rules_type>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, diagonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>));
                return rotate_directions_lfold<+90, -90>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, orthogonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>));
                return rotate_directions_lfold<+45, -45, +90, -90, +135, -135>(jumper);
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (... | scan(board::ray::rotate<Directions>(jumper)));
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                slide(jumper, builder.template path<board::ray::direction_v<Iterator>.degrees>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        auto slide(Iterator& jumper, set_type const path) const
        {
                assert(is_onboard(jumper));
                slide_dispatch(jumper, path, king_range_category_t<rules_type>{});
        }

        template<class Iterator>
        auto slide_dispatch(Iterator& jumper, set_type const /* path */, short_ranged_tag) const
        {
                ++jumper;
        }

        template<class Iterator>
        auto slide_dispatch(Iterator& jumper, set_type const path, long_ranged_tag) const
        {
                do ++jumper; while (is_onboard(jumper) && path.test(*jumper));
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && builder.current_targets(jumper)))
                        return false;

                assert(is_onboard(std::next(jumper)));
                capture(jumper);
                return true;
        }

        template<class Iterator>
        auto add(Iterator dest_sq) const
        {
                halt_dispatch(dest_sq, king_range_category_land_behind_piece_t<rules_type>{}, king_range_category_halt_behind_king_t<rules_type>{});
        }

        template<class Iterator>
        auto halt_dispatch(Iterator dest_sq, long_ranged_tag, short_ranged_tag) const
        {
                if (builder.is_last_jumped_king(*std::prev(dest_sq)))
                        halt_dispatch(dest_sq, short_ranged_tag{}, short_ranged_tag{});
                else
                        halt_dispatch(dest_sq,  long_ranged_tag{},  long_ranged_tag{});
        }

        template<class Iterator>
        auto halt_dispatch(Iterator dest_sq, short_ranged_tag, short_ranged_tag) const
        {
                add_jump(*dest_sq);
        }

        template<class Iterator>
        auto halt_dispatch(Iterator dest_sq, long_ranged_tag, long_ranged_tag) const
        {
                // builder.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(is_onboard(dest_sq) && builder.not_occupied(*dest_sq));
                do add_jump(*dest_sq++); while (is_onboard(dest_sq) && builder.not_occupied(*dest_sq));
        }

        auto add_jump(std::size_t const dest_sq) const
        {
                builder.finalize(dest_sq);
        }

        template<int Direction>
        auto along_ray(std::size_t const sq) const
        {
                return board::ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
