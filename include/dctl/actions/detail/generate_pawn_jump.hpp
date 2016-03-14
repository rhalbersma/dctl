#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, ToggleKingTargets, SetPromotion
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/detail/generate_king_jump.hpp>   // promote_en_passant
#include <dctl/actions/select/jump.hpp>                 // jumps
#include <dctl/board/angle.hpp>                         // _deg, rotate, inverse
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror, turn
#include <dctl/board/wave.hpp>                          // make_iterator, JumpTargets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/state/promotion.hpp>                     // is_promotion
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class Builder>
class Generate<ToMove, Piece::pawn, select::jump, Reverse, State, Builder>
{
        using  KingJumps = Generate<ToMove, Piece::king, select::jump, Reverse, State, Builder>;
        using board_type = board_t<Builder>;
        using rules_type = rules_t<Builder>;
        using   set_type =   set_t<Builder>;
        using action_type = typename Builder::action_type;

        template<int Direction>
        using jump_targets = JumpTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto bearing = bearing_v<board_type, ToMove, Reverse::value>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(bearing));

        State const& state;
        Builder& builder;
public:
        Generate(State const& s, Builder& b) noexcept
        :
                state{s},
                builder{b}
        {}

        auto operator()() const
        {
                if (pieces<ToMove, Piece::pawn>(state).any())
                        pawn_jump_king_dispatch(is_superior_rank_jump_t<rules_type>{});
        }
private:
        auto pawn_jump_king_dispatch(std::false_type) const
        {
                sources();
        }

        auto pawn_jump_king_dispatch(std::true_type) const
        {
                raii::ToggleKingTargets<Builder> guard{builder};
                sources();
        }

        auto sources() const
        {
                sources_dispatch(pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        auto sources_dispatch(forward_pawn_jump_tag, diagonal_jump_tag) const
        {
                directions_lfold<left_up, right_up>();
        }

        auto sources_dispatch(backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                directions_lfold<left_up, right_up, left_down, right_down>();
        }

        auto sources_dispatch(forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                directions_lfold<up, left_up, right_up, left, right>();
        }

        auto sources_dispatch(backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                directions_lfold<up, left_up, right_up, left, right, left_down, right_down, down>();
        }

        template<template<int> class... Directions>
        auto directions_lfold() const
        {
                return (..., targets<Directions<bearing.degrees()>{}>());
        }

        template<int Direction>
        auto targets() const
        {
                jump_targets<Direction>{}(
                        pieces<ToMove, Piece::pawn>(state),
                        builder.targets(),
                        builder.path()
                ).for_each([this](auto const& dest_sq){
                        first_target(along_ray<Direction>(dest_sq), action_type{state});
                });
        }

        template<class Iterator>
        auto first_target(Iterator jumper, action_type current) const
        {
                assert(is_onboard(jumper));
                raii::Launch<Builder> guard{builder, *std::prev(jumper)};
                current.pawn_jump_depart(*std::prev(jumper), state);
                capture(jumper, current);
        }

        template<class Iterator>
        auto capture(Iterator jumper, action_type current) const
        {
                assert(is_onboard(jumper));
                current.capture(*jumper, state);
                land(std::next(jumper), current);
        }

        template<class Iterator>
        auto land(Iterator jumper, action_type current) const
        {
                assert(is_onboard(jumper));
                try_promotion(jumper, current);
        }

        template<class Iterator>
        auto try_promotion(Iterator jumper, action_type current) const
        {
                try_promotion_dispatch(jumper, current, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, action_type current, stopped_promotion_tag) const
        {
                if (next_target(jumper, current))
                        return;
                if (is_promotion(*jumper))
                        return on_promotion(jumper, current);
                add_jump(*jumper, current);
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, action_type current, passing_promotion_tag) const
        {
                if (is_promotion(*jumper))
                        return on_promotion(jumper, current);
                try_next(jumper, current);
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper, action_type current) const
        {
                raii::SetPromotion<Builder> guard{builder};
                on_promotion_dispatch(jumper, current, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, action_type current, stopped_promotion_tag) const
        {
                add_jump(*jumper, current);
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, action_type current, passing_promotion_tag) const
        {
                king_jumps_dispatch(jumper, current, is_superior_rank_jump_t<rules_type>{});
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, action_type current, std::false_type) const
        {
                king_jumps_try_next(jumper, current);
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, action_type current, std::true_type) const
        {
                raii::ToggleKingTargets<Builder> guard{builder};
                king_jumps_try_next(jumper, current);
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper, action_type current) const
        {
                KingJumps{state, builder}.try_next(jumper, current, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_next(Iterator jumper, action_type current) const
        {
                if (!next_target(jumper, current))
                        add_jump(*jumper, current);
        }

        template<class Iterator>
        auto next_target(Iterator jumper, action_type current) const
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                return scan(jumper, current) | turn(jumper, current);
        }

        template<class Iterator>
        auto turn(Iterator jumper, action_type current) const
        {
                return turn_dispatch(jumper, current, pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, forward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>));
                return scan(ray::mirror<up<bearing.degrees()>{}>(jumper), current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>));
                return rotate_directions_lfold<+90, -90>(jumper, current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                static_assert(!is_down(direction_v<Iterator>));
                return turn_dispatch(jumper, current, angle_constant<direction_v<Iterator>>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, up<bearing.degrees()>) const
        {
                return turn_directions_lfold<left_up, right_up, left, right>(jumper, current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, left_up<bearing.degrees()>) const
        {
                return turn_directions_lfold<up, right_up, left, right>(jumper, current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, right_up<bearing.degrees()>) const
        {
                return turn_directions_lfold<up, left_up, left, right>(jumper, current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, left<bearing.degrees()>) const
        {
                return turn_directions_lfold<up, left_up, right_up>(jumper, current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, right<bearing.degrees()>) const
        {
                return turn_directions_lfold<up, left_up, right_up>(jumper, current);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, action_type current, backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                return rotate_directions_lfold<+45, -45, +90, -90, +135, -135>(jumper, current);
        }

        template<template<int> class... Directions, class Iterator>
        auto turn_directions_lfold(Iterator jumper, action_type current) const
        {
                return (... | scan(ray::turn<Directions<bearing.degrees()>{}>(jumper), current));
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper, action_type current) const
        {
                return (... | scan(ray::rotate<Directions>(jumper), current));
        }

        template<class Iterator>
        auto scan(Iterator jumper, action_type current) const
        {
                return is_en_prise(std::next(jumper), current);
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper, action_type current) const
        {
                if (!(is_onboard(jumper) && builder.targets(jumper, current)))
                        return false;

                assert(is_onboard(std::next(jumper)));
                capture(jumper, current);
                return true;
        }

        auto add_jump(std::size_t dest_sq, action_type current) const
        {
                current.pawn_jump_arrive(dest_sq, state);
                if (builder.is_promotion())
                        current.promote(dest_sq);
                current.king_captures(pieces<!ToMove, Piece::king>(state), state);
                builder.append_to(current);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static auto is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};


}       // namespace detail
}       // namespace core
}       // namespace dctl
