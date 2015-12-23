#pragma once
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // up, left_up, right_up, left, right, left_down, right_down, down
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/wave/patterns.hpp>                 // Sandwich
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_long_ranged_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Reverse, class Builder>
class Detect<ToMove, Piece::king, select::jump, Reverse, Builder>
{
        using   board_type = board_t<Builder>;
        using   rules_type = rules_t<Builder>;
        using     set_type =   set_t<Builder>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        Builder const& builder;

public:
        explicit Detect(Builder const& b) noexcept
        :
                builder{b}
        {}

        auto operator()(set_type const& active_kings) const noexcept
        {
                return active_kings.none() ? false : branch(active_kings);
        }

private:
        auto branch(set_type const& active_kings) const noexcept
        {
                return branch_dispatch(active_kings, jump_category_t<rules_type>{});
        }

        auto branch_dispatch(set_type const& active_kings, diagonal_jump_tag) const noexcept
        {
                return parallelize_lfold<left_up, right_up, left_down, right_down>(active_kings);
        }

        auto branch_dispatch(set_type const& active_kings, orthogonal_jump_tag) const noexcept
        {
                return parallelize_lfold<up, left_up, right_up, left, right, left_down, right_down, down>(active_kings);
        }

        template<template<int> class... Directions>
        auto parallelize_lfold(set_type const& active_kings) const
        {
                return (parallelize<Directions<orientation>{}>(active_kings) || ...);
        }

        template<int Direction>
        auto parallelize(set_type const& active_kings) const noexcept
        {
                return Sandwich<board_type, Direction, king_range_category_t<rules_type>>{}(
                        active_kings, builder.template targets<Direction>(), builder.path()
                ).any();
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
