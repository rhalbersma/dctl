#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, SetKingJump
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down, _deg, rotate, inverse
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                        // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // is_base_of, false_type, true_type

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class Builder, class Sequence>
class Generate<ToMove, Piece::king, select::jump, Reverse, State, Builder, Sequence>
{
        using board_type = board_t<Builder>;
        using rules_type = rules_t<Builder>;
        using   set_type =   set_t<Builder>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        State const& state;
        Builder& builder;
        Sequence& actions;
public:
        Generate(State const& s, Builder& b, Sequence& a) noexcept
        :
                state{s},
                builder{b},
                actions{a}
        {}

        auto operator()() const
        {
                raii::SetKingJump<Builder> guard{builder};
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
                state.pieces(ToMove, Piece::king).for_each([this](auto const& from_sq){
                        raii::Launch<Builder> guard{builder, from_sq};
                        source_dispatch(from_sq, jump_category_t<rules_type>{});
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
                return (first_target(along_ray<Directions<orientation>{}>(from_sq)) , ...);
        }

        template<class Iterator>
        auto first_target(Iterator jumper) const
        {
                slide(jumper, builder.template path<ray::direction_v<Iterator>>());
                if (is_onboard(jumper) && builder.targets(jumper)) {
                        assert(is_onboard(std::next(jumper)));
                        capture(jumper);
                }
        }

        template<class Iterator>
        auto capture(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Capture<Builder> guard{builder, *jumper};
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
                return reverse_dispatch(jumper, is_reversible_king_jump_direction_t<rules_type>{});
        }

        template<class Iterator>
        auto reverse_dispatch(Iterator jumper, std::false_type) const
        {
                return scan_turn(jumper);
        }

        template<class Iterator>
        auto reverse_dispatch(Iterator jumper, std::true_type) const
        {
                return scan_turn(jumper) | reverse(jumper);
        }

        template<class Iterator>
        auto reverse(Iterator jumper) const
        {
                static_assert(is_reversible_king_jump_direction_v<rules_type>);
                return scan(ray::rotate<180_deg>(jumper));
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
                assert(is_onboard(jumper) && builder.path(*jumper));
                auto found_next = turn(jumper);
                auto slider = std::next(jumper);
                while (is_onboard(slider) && builder.path(*slider)) {
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
                return rotate_directions_lfold<+90_deg, -90_deg>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, orthogonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>));
                return rotate_directions_lfold<+45_deg, -45_deg, +90_deg, -90_deg, +135_deg, -135_deg>(jumper);
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (scan(ray::rotate<Directions>(jumper)) | ...);
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                slide(jumper, builder.template path<ray::direction_v<Iterator>>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        auto slide(Iterator& jumper, set_type const& path) const
        {
                assert(is_onboard(jumper));
                slide_dispatch(jumper, path, king_range_category_t<rules_type>{});
        }

        template<class Iterator>
        auto slide_dispatch(Iterator& jumper, set_type const& /* path */, short_ranged_tag) const
        {
                ++jumper;
        }

        template<class Iterator>
        auto slide_dispatch(Iterator& jumper, set_type const& path, long_ranged_tag) const
        {
                do ++jumper; while (is_onboard(jumper) && path.test(*jumper));
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && builder.targets(jumper)))
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
                if (builder.is_last_jumped(Piece::king, *std::prev(dest_sq)))
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
                assert(is_onboard(dest_sq) && builder.path(*dest_sq));
                do add_jump(*dest_sq++); while (is_onboard(dest_sq) && builder.path(*dest_sq));
        }

        auto add_jump(std::size_t dest_sq) const
        {
                builder.finish(dest_sq);
                builder.append_to(actions);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
