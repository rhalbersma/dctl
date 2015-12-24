#pragma once
#include <dctl/actions/detail/raii.hpp>                 // ToggleKingTargets
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave/patterns.hpp>                 // Targets
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/rule_traits.hpp>                         // is_backward_pawn_jump, is_orthogonal_jump, is_pawns_jump_only_pawns
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class Builder>
class Detect<ToMove, Piece::pawn, select::jump, Reverse, Builder>
{
        using   board_type = board_t<Builder>;
        using   rules_type = rules_t<Builder>;
        using     set_type =   set_t<Builder>;

        template<int Direction>
        using jump_targets = JumpTargets<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        Builder& builder;

public:
        explicit Detect(Builder& b) noexcept
        :
                builder{b}
        {}

        auto operator()(set_type const& active_pawns) const noexcept
        {
                return active_pawns.any() ? king_targets_dispatch(active_pawns, is_pawns_jump_only_pawns_t<rules_type>{}) : false;
        }

private:
        // pawns that can capture kings
        auto king_targets_dispatch(set_type const& active_pawns, std::false_type) const noexcept
        {
                return sources(active_pawns);
        }

        // pawns that cannot capture kings
        auto king_targets_dispatch(set_type const& active_pawns, std::true_type) const noexcept
        {
                raii::ToggleKingTargets<Builder> guard{builder};
                return sources(active_pawns);
        }

        auto sources(set_type const& active_pawns) const
        {
                return sources_dispatch(active_pawns, pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        auto sources_dispatch(set_type const& active_pawns, forward_pawn_jump_tag, diagonal_jump_tag) const noexcept
        {
                return directions_lfold<left_up, right_up>(active_pawns);
        }

        auto sources_dispatch(set_type const& active_pawns, backward_pawn_jump_tag, diagonal_jump_tag) const noexcept
        {
                return directions_lfold<left_up, right_up, left_down, right_down>(active_pawns);
        }

        auto sources_dispatch(set_type const& active_pawns, forward_pawn_jump_tag, orthogonal_jump_tag) const noexcept
        {
                return directions_lfold<up, left_up, right_up, left, right>(active_pawns);
        }

        auto sources_dispatch(set_type const& active_pawns, backward_pawn_jump_tag, orthogonal_jump_tag) const noexcept
        {
                return directions_lfold<up, left_up, right_up, left, right, left_down, right_down, down>(active_pawns);
        }

        template<template<int> class... Directions>
        auto directions_lfold(set_type const& active_pawns) const
        {
                return (targets<Directions<orientation>{}>(active_pawns) || ...);
        }

        template<int Direction>
        auto targets(set_type const& active_pawns) const noexcept
        {
                return jump_targets<Direction>{}(
                        active_pawns, builder.template targets<Direction>(), builder.path()
                ).any();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
