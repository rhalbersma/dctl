#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, ToggleKingTargets, SetPromotion
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/detail/generate_king_jump.hpp>   // promote_en_passant
#include <dctl/actions/select/jump.hpp>                 // jumps
#include <dctl/board/angle.hpp>                         // _deg, rotate, inverse
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror, turn
#include <dctl/board/wave.hpp>                          // make_iterator
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_pawns_jump_only_pawns_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/state/promotion.hpp>                     // is_promotion
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class State, class Builder, class Sequence>
class Generate<ToMove, Piece::pawn, select::jump, Reverse, State, Builder, Sequence>
{
        using  KingJumps = Generate<ToMove, Piece::king, select::jump, Reverse, State, Builder, Sequence>;
        using board_type = board_t<Builder>;
        using rules_type = rules_t<Builder>;
        using   set_type =   set_t<Builder>;

        template<int Direction>
        using jump_targets = JumpTargets<board_type, Direction, short_ranged_tag>;

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
                if (state.pieces(ToMove, Piece::pawn).any())
                        pawn_jump_king_dispatch(is_pawns_jump_only_pawns_t<rules_type>{});
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
                return (targets<Directions<orientation>{}>() , ...);
        }

        template<int Direction>
        auto targets() const
        {
                jump_targets<Direction>{}(
                        state.pieces(ToMove, Piece::pawn), builder.remaining_targets(), builder.path()
                ).for_each([this](auto const& dest_sq){
                        first_target(along_ray<Direction>(dest_sq));
                });
        }

        template<class Iterator>
        auto first_target(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Launch<Builder> guard{builder, *std::prev(jumper)};
                capture(jumper);
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
                raii::SetPromotion<Builder> guard{builder};
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
                king_jumps(jumper);
        }

        template<class Iterator>
        auto king_jumps(Iterator jumper) const
        {
                king_jumps_dispatch(jumper, is_pawns_jump_only_pawns_t<rules_type>{});
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, std::false_type) const
        {
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, std::true_type) const
        {
                raii::ToggleKingTargets<Builder> guard{builder};
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper) const
        {
                KingJumps{state, builder, actions}.try_next(jumper, promotion_category_t<rules_type>{});
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
                return scan(ray::mirror<up<orientation>{}>(jumper));
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>));
                return rotate_directions_lfold<+90_deg, -90_deg>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                static_assert(!is_down(direction_v<Iterator>));
                return turn_dispatch(jumper, angle_constant<direction_v<Iterator>>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, up<orientation>) const
        {
                return turn_directions_lfold<left_up, right_up, left, right>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, left_up<orientation>) const
        {
                return turn_directions_lfold<up, right_up, left, right>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, right_up<orientation>) const
        {
                return turn_directions_lfold<up, left_up, left, right>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, left<orientation>) const
        {
                return turn_directions_lfold<up, left_up, right_up>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, right<orientation>) const
        {
                return turn_directions_lfold<up, left_up, right_up>(jumper);
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                return rotate_directions_lfold<+45_deg, -45_deg, +90_deg, -90_deg, +135_deg, -135_deg>(jumper);
        }

        template<template<int> class... Directions, class Iterator>
        auto turn_directions_lfold(Iterator jumper) const
        {
                return (scan(ray::turn<Directions<orientation>{}>(jumper)) | ...);
        }

        template<int... Directions, class Iterator>
        auto rotate_directions_lfold(Iterator jumper) const
        {
                return (scan(ray::rotate<Directions>(jumper)) | ...);
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                return is_en_prise(std::next(jumper));
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

        static auto is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
