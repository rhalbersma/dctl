#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/raii.hpp>                    // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/core/actions/detail/builder.hpp>                 // Builder
#include <dctl/core/actions/detail/generate_primary_fwd.hpp>    // generate (primary template)
#include <dctl/core/actions/detail/generate_king_jump.hpp>      // promote_en_passant
#include <dctl/core/actions/select/jump.hpp>                    // jumps
#include <dctl/core/board/angle.hpp>                            // rotate, inverse
#include <dctl/core/board/bearing.hpp>                          // bearing
#include <dctl/core/board/jump_sources.hpp>                     // jump_sources
#include <dctl/core/board/ray.hpp>                              // make_iterator, rotate, mirror, turn
#include <dctl/core/state/color_piece.hpp>                      // color, color_, pawns_, king_
#include <dctl/core/rules/type_traits.hpp>                      // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/util/meta.hpp>                                   // map_reduce, comma, bit_or, tuple_c, int_c
#include <dctl/util/type_traits.hpp>                            // action_t, board_t, rules_t, set_t
#include <cassert>                                              // assert
#include <iterator>                                             // next
#include <type_traits>                                          // is_same

namespace dctl::core {
namespace detail {

template<color Side, class Reverse, class State, class Builder>
class generate<color_<Side>, pawns_, select::jump, Reverse, State, Builder>
{
        using  king_jumps = generate<color_<Side>, kings_, select::jump, Reverse, State, Builder>;
        using  board_type = board_t<Builder>;
        using  rules_type = rules_t<Builder>;
        using    set_type =   set_t<Builder>;
        constexpr static auto orientation = bearing_v<board_type, color_<Side>, Reverse>;
        using pawn_jump_directions = std::conditional_t<
                is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>,
                std::tuple<right<orientation>, right_up<orientation>, up<orientation>, left_up<orientation>, left<orientation>, left_down<orientation>, down<orientation>, right_down<orientation>>,
                std::conditional_t<
                        !is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>,
                        std::tuple<right<orientation>, right_up<orientation>, up<orientation>, left_up<orientation>, left<orientation>>,
                        std::conditional_t<
                                is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>,
                                std::tuple<right_up<orientation>, left_up<orientation>, left_down<orientation>, right_down<orientation>>,
                                std::tuple<right_up<orientation>, left_up<orientation>>
                        >
                >
        >;

        template<class Iterator>
        constexpr static auto direction_v = rotate(ray::direction_v<Iterator>, inverse(angle{orientation}));

        Builder& m_builder;
public:
        explicit generate(Builder& b) noexcept
        :
                m_builder{b}
        {}

        auto operator()() const
        {
                if (auto const pawns = m_builder.pieces(color_c<Side>, pawns_c); !pawns.empty()) {
                        if constexpr (is_superior_rank_jump_v<rules_type>) { m_builder.toggle_king_targets(); }
                        meta::map_reduce<pawn_jump_directions, meta::comma>{}([&, this](auto direction) {
                                constexpr auto Direction = decltype(direction){};
                                jump_sources<board_type, Direction, short_ranged_tag>{}(pawns, m_builder.targets(), m_builder.pieces(empty_c)).consume([this](auto const from_sq) {
                                        raii::launch<Builder> guard{m_builder, from_sq};
                                        capture(std::next(ray::make_iterator<board_type, Direction>(from_sq)));
                                });
                        });
                        if constexpr (is_superior_rank_jump_v<rules_type>) { m_builder.toggle_king_targets(); }
                }
        }
private:
        template<class Iterator>
        auto capture(Iterator jumper) const
                -> void
        {
                raii::capture<Builder> guard{m_builder, *jumper};
                land(std::next(jumper));
        }

        template<class Iterator>
        auto land(Iterator jumper) const
        {
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (board_type::promotion(Side).contains(*jumper)) {
                                return on_promotion(jumper);
                        }
                        if (next_target(jumper)) {
                                return;
                        }
                } else {
                        if (next_target(jumper)) {
                                return;
                        }
                        if (board_type::promotion(Side).contains(*jumper)) {
                                return on_promotion(jumper);
                        }
                }
                m_builder.finalize(*jumper);
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper) const
        {
                raii::promotion<Builder> guard{m_builder};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        return on_king_jump(jumper);
                } else {
                        return m_builder.finalize(*jumper);
                }
        }

        template<class Iterator>
        auto on_king_jump(Iterator jumper) const
        {
                static_assert(is_passing_promotion_v<rules_type>);
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        raii::toggle_king_targets<Builder> guard{m_builder};
                        king_jumps{m_builder}.try_next_passing_promotion(jumper);
                } else {
                        king_jumps{m_builder}.try_next_passing_promotion(jumper);
                }
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
                if constexpr (is_backward_pawn_jump_v<rules_type>) {
                        using rotation_angles = std::conditional_t<
                                is_orthogonal_jump_v<rules_type>,
                                meta::tuple_c<-135, -90, -45, +45, +90, +135>,
                                meta::tuple_c<-90, +90>
                        >;
                        return meta::map_reduce<rotation_angles, meta::bit_or>{}([jumper, this](auto direction) {
                                return scan(ray::rotate<decltype(direction){}>(jumper));
                        });
                } else if constexpr (is_orthogonal_jump_v<rules_type>) {
                        static_assert(!is_down(direction_v<Iterator>));
                        using turn_directions = meta::switch_<
                                meta::int_c<direction_v<Iterator>>,
                                meta::case_<   right<orientation>, std::tuple<right_up<orientation>, up<orientation>, left_up<orientation>>>,
                                meta::case_<right_up<orientation>, std::tuple<right<orientation>, up<orientation>, left_up<orientation>, left<orientation>>>,
                                meta::case_<      up<orientation>, std::tuple<right<orientation>, right_up<orientation>, left_up<orientation>, left<orientation>>>,
                                meta::case_< left_up<orientation>, std::tuple<right<orientation>, right_up<orientation>, up<orientation>, left<orientation>>>,
                                meta::case_<    left<orientation>, std::tuple<right_up<orientation>, up<orientation>, left_up<orientation>>>
                        >;
                        return meta::map_reduce<turn_directions, meta::bit_or>{}([jumper, this](auto direction) {
                                return scan(ray::turn<decltype(direction){}>(jumper));
                        });
                } else {
                        static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>));
                        return scan(ray::mirror<up<orientation>{}>(jumper));
                }
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                if (m_builder.has_pawn_target(jumper)) {
                        capture(std::next(jumper));
                        return true;
                }
                return false;
        }
};

}       // namespace detail
}       // namespace dctl::core
