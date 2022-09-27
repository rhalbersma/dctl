#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/core/state/color.hpp>
#include <dctl/core/state/piece.hpp>
#include <dctl/core/state/position.hpp>
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/array.hpp>                       // or_empty
#include <cassert>                              // assert
#include <concepts>                             // same_as
#include <cstdint>                              // uint64_t
#include <tuple>                                // tie
#include <utility>                              // forward

namespace dctl::core {

struct nullmove_t {};
inline constexpr auto nullmove = nullmove_t{};

namespace detail {
namespace block_adl {

template<class Board>
struct base_position
{
        using position_type = bwk::position<Board>;
        position_type m_position;
};

struct base_color
{
        color m_color;
};

template<class Board>
struct most_recently_pushed_king {};

template<class Rules, class Board>
using conditional_base_mrpk = xstd::or_empty<
        is_restricted_king_move_v<Rules>,
        most_recently_pushed_king<Board>
>;

}       // namespace block_adl

using block_adl::base_position;
using block_adl::base_color;
using block_adl::conditional_base_mrpk;

}       // namespace detail

template<class Rules, class Board>
class basic_state
:
        detail::base_position<Board>,
        detail::conditional_base_mrpk<Rules, Board>,
        detail::base_color
{
        using base_position = detail::base_position<Board>;
        using conditional_base_mrpk = detail::conditional_base_mrpk<Rules, Board>;
        using base_color = detail::base_color;
public:
        using    rules_type = Rules;
        using    board_type = Board;
        using position_type = position_t<base_position>;
        using     mask_type = mask_t<position_type>;
        using      set_type =  set_t<mask_type>;

private:
        constexpr auto assert_invariants() const noexcept
        {
                assert(true);
        }

public:
        basic_state(position_type const& position, color const to_move)
        :
                base_position{position},
                conditional_base_mrpk{},
                base_color{to_move}
        {
                assert_invariants();
        }

        constexpr bool operator==(basic_state const& other) const noexcept
        {
                constexpr auto tied = [](auto const& s) {
                        return std::tie(s.m_position, s.m_color);
                };
                return tied(*this) == tied(other);
        }

        constexpr static auto initial(int const separation = initial_position_gap_v<Rules> + Board::height % 2)
                -> basic_state
        {
                auto const black_pawns = mask_type::initial(black_c, separation);
                auto const white_pawns = mask_type::initial(white_c, separation);
                return {{black_pawns, white_pawns}, color::white};
        }

        template<class Action>
        auto make(Action const& a)
        {
                static_assert(std::same_as<rules_type, rules_t<Action>>);
                static_assert(std::same_as<board_type, board_t<Action>>);
                this->m_position.make(to_move(), a);
                pass_turn();
                assert_invariants();
        }

        auto make(nullmove_t)
        {
                pass_turn();
                assert_invariants();
        }

        template<class Action>
        auto undo(Action const& a)
        {
                static_assert(std::same_as<rules_type, rules_t<Action>>);
                static_assert(std::same_as<board_type, board_t<Action>>);
                pass_turn();
                this->m_position.undo(to_move(), a);
                assert_invariants();
        }

        auto undo(nullmove_t)
        {
                pass_turn();
                assert_invariants();
        }

        template<class... Args>
                requires (sizeof...(Args) <= 2)
        constexpr auto pieces(Args&&... args) const noexcept
        {
                return this->m_position.pieces(std::forward<Args>(args)...);
        }

        template<color Side, piece Type>
        constexpr auto targets(color_<Side>, piece_<Type>) const noexcept
        {
                if constexpr (Type == piece::pawn && is_superior_rank_jump_v<rules_type>) {
                        return pieces(color_c<!Side>, pawn_c);
                } else {
                        return pieces(!color_c<Side>);
                }
        }

        template<class... Args>
                requires (sizeof...(Args) <= 2)
        constexpr auto num_pieces(Args&&... args) const noexcept
        {
                return this->m_position.num_pieces(std::forward<Args>(args)...);
        }

        constexpr auto to_move() const noexcept
        {
                return m_color;
        }

private:
        constexpr auto pass_turn() noexcept
        {
                m_color = !m_color;
        }
};

}       // namespace dctl::core
