#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/detail/generate_king_jump.hpp>   // promote_en_passant
#include <dctl/actions/select/jump.hpp>                 // jumps
#include <dctl/board/angle.hpp>                         // _deg, rotate, inverse
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror, turn
#include <dctl/color.hpp>                               // color
#include <dctl/mask/jump_sources.hpp>                   // jump_sources
#include <dctl/mask/promotion.hpp>                      // is_promotion
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/utility/type_traits.hpp>                 // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev

namespace dctl {
namespace core {
namespace detail {

template<color ToMove, class Reverse, class State, class Builder>
class generate<ToMove, piece::pawn, select::jump, Reverse, State, Builder>
{
        using  king_jumps = generate<ToMove, piece::king, select::jump, Reverse, State, Builder>;
        using action_type = action_t<Builder>;
        using  board_type =  board_t<Builder>;
        using  rules_type =  rules_t<Builder>;
        using    set_type =    set_t<Builder>;

        template<int Direction>
        using jump_sources = mask::jump_sources<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(board::ray::direction_v<Iterator>, inverse(bearing));

        Builder& builder;
public:
        generate(Builder& b) noexcept
        :
                builder{b}
        {}

        auto operator()() const
        {
                if (builder.active_pawns().any())
                        pawn_jump_king_dispatch(rank_jump_category_t<rules_type>{});
        }
private:
        auto pawn_jump_king_dispatch(inferior_rank_jump_tag) const
        {
                directions();
        }

        auto pawn_jump_king_dispatch(superior_rank_jump_tag) const
        {
                raii::toggle_king_targets<Builder> guard{builder};
                directions();
        }

        auto directions() const
        {
                directions_dispatch(pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        auto directions_dispatch(forward_pawn_jump_tag, diagonal_jump_tag) const
        {
                directions_lfold<right_up, left_up>();
        }

        auto directions_dispatch(backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                directions_lfold<right_up, left_up, left_down, right_down>();
        }

        auto directions_dispatch(forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                directions_lfold<right, right_up, up, left_up, left>();
        }

        auto directions_dispatch(backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                directions_lfold<right, right_up, up, left_up, left, left_down, down, right_down>();
        }

        template<template<int> class... Directions>
        auto directions_lfold() const
        {
                (... , sources<Directions<bearing.degrees>{}>());
        }

        template<int Direction>
        auto sources() const
        {
                jump_sources<Direction>{}(
                        builder.active_pawns(),
                        builder.current_targets(),
                        builder.not_occupied()
                ).for_each([this](auto const from_sq){
                        this->jump(this->along_ray<Direction>(from_sq));
                });
        }

        template<class Iterator>
        auto jump(Iterator const jumper) const
        {
                assert(is_onboard(jumper));
                raii::launch<Builder> guard{builder, *jumper};
                capture(std::next(jumper));
        }

        template<class Iterator>
        auto capture(Iterator const jumper) const
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
                try_promotion(jumper);
        }

        template<class Iterator>
        auto try_promotion(Iterator jumper) const
        {
                try_promotion_dispatch(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, stopped_promotion_tag) const
        {
                if (next_target(jumper))
                        return;
                if (is_promotion(*jumper))
                        return on_promotion(jumper);
                add_jump(*jumper);
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, passing_promotion_tag) const
        {
                if (is_promotion(*jumper))
                        return on_promotion(jumper);
                try_next(jumper);
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper) const
        {
                raii::promotion<Builder> guard{builder};
                on_promotion_dispatch(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, stopped_promotion_tag) const
        {
                add_jump(*jumper);
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, passing_promotion_tag) const
        {
                king_jumps_dispatch(jumper, rank_jump_category_t<rules_type>{});
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, inferior_rank_jump_tag) const
        {
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, superior_rank_jump_tag) const
        {
                raii::toggle_king_targets<Builder> guard{builder};
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper) const
        {
                king_jumps{builder}.try_next(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!next_target(jumper))
                        add_jump(*jumper);
        }

        template<class Iterator>
        auto next_target(Iterator jumper) const
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                return turn_dispatch(jumper, pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, forward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>));
                return scan(board::ray::mirror<up<bearing.degrees>{}>(jumper));
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>));
                return rotate_directions_lfold<+90, -90>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                static_assert(!is_down(direction_v<Iterator>));
                return turn_dispatch(jumper, angle_constant<direction_v<Iterator>>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, up<bearing.degrees>) const
        {
                return turn_directions_lfold<left_up, right_up, left, right>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, left_up<bearing.degrees>) const
        {
                return turn_directions_lfold<up, right_up, left, right>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, right_up<bearing.degrees>) const
        {
                return turn_directions_lfold<up, left_up, left, right>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, left<bearing.degrees>) const
        {
                return turn_directions_lfold<up, left_up, right_up>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, right<bearing.degrees>) const
        {
                return turn_directions_lfold<up, left_up, right_up>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                return rotate_directions_lfold<+45, -45, +90, -90, +135, -135>(jumper);
        }

        template<template<int> class... Directions, class Iterator>
        auto turn_directions_lfold(Iterator jumper) const
        {
                return (... | scan(board::ray::turn<Directions<bearing.degrees>{}>(jumper)));
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (... | scan(board::ray::rotate<Directions>(jumper)));
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                return is_en_prise(std::next(jumper));
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

        auto add_jump(std::size_t const dest_sq) const
        {
                builder.finalize(dest_sq);
        }

        template<int Direction>
        auto along_ray(std::size_t const sq) const noexcept
        {
                return board::ray::make_iterator<board_type, Direction>(sq);
        }

        auto is_promotion(std::size_t const sq) const // Throws: Nothing.
        {
                return mask::promotion_v<board_type, ToMove>.test(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
