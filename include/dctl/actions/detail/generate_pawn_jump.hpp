#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, Toggleking_targets, Setpromotion
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/detail/generate_king_jump.hpp>   // promote_en_passant
#include <dctl/actions/select/jump.hpp>                 // jumps
#include <dctl/board/angle.hpp>                         // rotate, inverse
#include <dctl/board/bearing.hpp>                       // bearing
#include <dctl/board/mask/jump_sources.hpp>             // jump_sources
#include <dctl/board/mask/promotion.hpp>                // is_promotion
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror, turn
#include <dctl/color_piece.hpp>                         // color, color_, pawn_, king_
#include <dctl/rule_traits.hpp>                         // is_superior_rank_jump_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/utility/type_traits.hpp>                 // action_t, board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // is_same

namespace dctl {
namespace detail {

template<color Side, class Reverse, class State, class Builder>
class Generate<color_<Side>, pawn_, select::jump, Reverse, State, Builder>
{
        using to_move_ = color_<Side>;
        static constexpr auto to_move_c = color_c<Side>;
        static constexpr auto piece_c = pawn_c;
        using  king_jumps = Generate<to_move_, king_, select::jump, Reverse, State, Builder>;
        using  board_type =  board_t<Builder>;
        using  rules_type =  rules_t<Builder>;
        using    set_type =    set_t<Builder>;

        template<int Direction>
        using jump_sources = board::mask::jump_sources<board_type, Direction, short_ranged_tag>;

        static constexpr auto orientation = board::bearing_v<board_type, to_move_, Reverse>.value();

        template<class Iterator>
        static constexpr auto direction_v = rotate(board::ray::direction_v<Iterator>, inverse(board::angle{orientation}));

        Builder& m_builder;
public:
        explicit Generate(Builder& b) noexcept
        :
                m_builder{b}
        {}

        auto operator()() const
        {
                if (m_builder.pieces(to_move_c, piece_c).none()) {
                        return;
                }
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        raii::toggle_king_targets<Builder> guard{m_builder};
                        directions();
                } else {
                        directions();
                }
        }
private:
        auto directions() const
        {
                if constexpr (!is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right_up, board::left_up>();
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right_up, board::left_up, board::left_down, board::right_down>();
                }
                if constexpr (!is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right, board::right_up, board::up, board::left_up, board::left>();
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && is_orthogonal_jump_v<rules_type>) {
                        return directions_lfold<board::right, board::right_up, board::up, board::left_up, board::left, board::left_down, board::down, board::right_down>();
                }
        }

        template<template<int> class... Directions>
        auto directions_lfold() const
        {
                (... , sources<Directions<orientation>{}>());
        }

        template<int Direction>
        auto sources() const
        {
                jump_sources<Direction>{}(
                        m_builder.pieces(to_move_c, piece_c),
                        m_builder.targets(),
                        m_builder.pieces(none_c)
                ).for_each([this](auto const from_sq){
                        jump(along_ray<Direction>(from_sq));
                });
        }

        template<class Iterator>
        auto jump(Iterator const jumper) const
        {
                assert(is_onboard(jumper));
                raii::launch<Builder> guard{m_builder, *jumper};
                capture(std::next(jumper));
        }

        template<class Iterator>
        auto capture(Iterator const jumper) const
                -> void
        {
                assert(is_onboard(jumper));
                raii::capture<Builder> guard{m_builder, *jumper};
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
                if constexpr (is_passing_promotion_v<rules_type>) {
                        if (is_promotion(*jumper))
                                return on_promotion(jumper);
                        return try_next(jumper);
                } else {
                        if (next_target(jumper))
                                return;
                        if (is_promotion(*jumper))
                                return on_promotion(jumper);
                        return add_jump(*jumper);
                }
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper) const
        {
                raii::promotion<Builder> guard{m_builder};
                if constexpr (is_passing_promotion_v<rules_type>) {
                        return on_king_jump(jumper);
                } else {
                        return add_jump(*jumper);
                }
        }

        template<class Iterator>
        auto on_king_jump(Iterator jumper) const
        {
                static_assert(is_passing_promotion_v<rules_type>);
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        raii::toggle_king_targets<Builder> guard{m_builder};
                        king_jumps_try_next(jumper);
                } else {
                        king_jumps_try_next(jumper);
                }
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper) const
        {
                static_assert(is_passing_promotion_v<rules_type>);
                king_jumps{m_builder}.try_next(jumper, passing_promotion_tag{});
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
                if constexpr (!is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>));
                        return scan(board::ray::mirror<board::up<orientation>{}>(jumper));
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> && !is_orthogonal_jump_v<rules_type>) {
                        static_assert(is_diagonal(direction_v<Iterator>));
                        return rotate_directions_lfold<-90, +90>(jumper);
                }
                if constexpr (!is_backward_pawn_jump_v<rules_type> &&  is_orthogonal_jump_v<rules_type>) {
                        static_assert(!is_down(direction_v<Iterator>));

                        if constexpr (direction_v<Iterator> == board::right<orientation>{}) {
                                return turn_directions_lfold<board::right_up, board::up, board::left_up>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == board::right_up<orientation>{}) {
                                return turn_directions_lfold<board::right, board::up, board::left_up, board::left>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == board::up<orientation>{}) {
                                return turn_directions_lfold<board::right, board::right_up, board::left_up, board::left>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == board::left_up<orientation>{}) {
                                return turn_directions_lfold<board::right, board::right_up, board::up, board::left>(jumper);
                        }
                        if constexpr (direction_v<Iterator> == board::left<orientation>{}) {
                                return turn_directions_lfold<board::right_up, board::up, board::left_up>(jumper);
                        }
                }
                if constexpr (is_backward_pawn_jump_v<rules_type> &&  is_orthogonal_jump_v<rules_type>) {
                        return rotate_directions_lfold<-135, -90, -45, +45, +90, +135>(jumper);
                }
        }

        template<template<int> class... Directions, class Iterator>
        auto turn_directions_lfold(Iterator jumper) const
        {
                return (... | scan(board::ray::turn<Directions<orientation>{}>(jumper)));
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
                if (!(is_onboard(jumper) && m_builder.is_target(jumper)))
                        return false;

                assert(is_onboard(std::next(jumper)));
                capture(jumper);
                return true;
        }

        auto add_jump(std::size_t const dest_sq) const
        {
                m_builder.finalize(dest_sq);
        }

        template<int Direction>
        auto along_ray(std::size_t const sq) const noexcept
        {
                return board::ray::make_iterator<board_type, Direction>(sq);
        }

        auto is_promotion(std::size_t const sq) const // Throws: Nothing.
        {
                return board::mask::promotion_v<board_type, to_move_>.test(sq);
        }
};

}       // namespace detail
}       // namespace dctl
