#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, Setking_jump
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down, rotate, inverse
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror
#include <dctl/color_piece.hpp>                         // Color, color_constant, king_
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                        // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/utility/type_traits.hpp>                 // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // is_same

namespace dctl {
namespace detail {

template<Color Side, class Reverse, class State, class Builder>
class Generate<color_constant<Side>, king_, select::jump, Reverse, State, Builder>
{
        using to_move_ = color_constant<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = king_c;
        using  board_type =  board_t<Builder>;
        using  rules_type =  rules_t<Builder>;
        using    set_type =    set_t<Builder>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.degrees();

        template<class Iterator>
        static constexpr auto direction_v = rotate(board::ray::direction_v<Iterator>, inverse(board::Angle{orientation}));

        Builder& builder;
public:
        explicit Generate(Builder& b) noexcept
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
                static_assert(is_passing_promotion_v<rules_type>);
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::king));
                try_next(jumper);
        }
private:
        auto sources() const
        {
                builder.pieces(to_move_c, piece_c).for_each([this](auto const from_sq) {
                        raii::launch<Builder> guard{builder, from_sq};
                        if constexpr (is_orthogonal_jump_v<rules_type>) {
                                directions_lfold<board::right, board::right_up, board::up, board::left_up, board::left, board::left_down, board::down, board::right_down>(from_sq);
                        } else {
                                directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>(from_sq);
                        }
                });
        }

        template<template<int> class... Directions>
        auto directions_lfold(std::size_t from_sq) const
        {
                (... , first_target(along_ray<Directions<orientation>{}>(from_sq)));
        }

        template<class Iterator>
        auto first_target(Iterator jumper) const
        {
                slide(jumper, builder.template path<board::ray::direction_v<Iterator>.degrees()>());
                if (is_onboard(jumper) && builder.is_target(jumper)) {
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
                        halt(jumper);
        }

        template<class Iterator>
        auto next_target(Iterator jumper) const
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                if constexpr (is_reverse_king_jump_v<rules_type>) {
                        return scan_turn(jumper) | reverse(jumper);
                } else {
                        return scan_turn(jumper);
                }
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
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return scan(jumper) | turn(jumper);
                } else {
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
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                if constexpr (is_orthogonal_jump_v<rules_type>) {
                        static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>));
                        return rotate_directions_lfold<-135, -90, -45, +45, +90, +135>(jumper);
                } else {
                        static_assert(is_diagonal(direction_v<Iterator>));
                        return rotate_directions_lfold<-90, +90>(jumper);
                }
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (... | scan(board::ray::rotate<Directions>(jumper)));
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                slide(jumper, builder.template path<board::ray::direction_v<Iterator>.degrees()>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        auto slide(Iterator& jumper, set_type const path) const
        {
                assert(is_onboard(jumper));
                if constexpr (is_long_ranged_king_v<rules_type>) {
                        do ++jumper; while (is_onboard(jumper) && path.test(*jumper));
                } else {
                        ++jumper;
                }
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && builder.is_target(jumper)))
                        return false;

                assert(is_onboard(std::next(jumper)));
                capture(jumper);
                return true;
        }

        template<class Iterator>
        auto halt(Iterator dest_sq) const
        {
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && is_halt_behind_king_v<rules_type>) {
                        if (builder.is_last_jumped_king(*std::prev(dest_sq))) {
                                return add_halting_jump(*dest_sq);
                        } else {
                                return add_sliding_jumps(dest_sq);
                        }
                }
                if constexpr (is_long_ranged_king_v<rules_type> && !is_land_behind_piece_v<rules_type> && !is_halt_behind_king_v<rules_type>) {
                        return add_sliding_jumps(dest_sq);
                }
                if constexpr (!is_long_ranged_king_v<rules_type> || is_land_behind_piece_v<rules_type>) {
                        return add_halting_jump(*dest_sq);
                }
        }

        template<class Iterator>
        auto add_sliding_jumps(Iterator dest_sq) const
        {
                // builder.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(is_onboard(dest_sq) && builder.not_occupied(*dest_sq));
                do add_halting_jump(*dest_sq++); while (is_onboard(dest_sq) && builder.not_occupied(*dest_sq));
        }

        auto add_halting_jump(std::size_t const dest_sq) const
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
}       // namespace dctl
