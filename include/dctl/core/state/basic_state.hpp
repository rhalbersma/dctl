#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>
#include <dctl/core/rules/type_traits.hpp>      // is_restricted_king_move_v
#include <dctl/core/state/color.hpp>
#include <dctl/core/state/piece.hpp>
#include <dctl/core/state/position.hpp>
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/array.hpp>                       // or_empty
#include <cassert>                              // assert
#include <utility>                              // forward

namespace dctl::core {

struct nullmove_t {};
inline constexpr auto nullmove = nullmove_t{};

template<class Board>
struct most_recently_pushed_king
{
        bool operator==(most_recently_pushed_king const&) const = default;
};

template<class Rules, class Board>
class basic_state
{
public:
        using    rules_type = Rules;
        using    board_type = Board;
        using position_type = bwk::position<Board>;
        using     mask_type = mask_t<position_type>;
        using      set_type =  set_t<mask_type>;

private:
        position_type m_position;
        xstd::or_empty<
                is_restricted_king_move_v<Rules>,
                most_recently_pushed_king<Board>
        > m_mrpk [[no_unique_address]];
        color m_color;

        [[nodiscard]] constexpr auto assert_invariants() const noexcept
        {
                assert(true);
        }

        constexpr auto pass_turn() noexcept
        {
                m_color = !m_color;
        }

public:
        [[nodiscard]] constexpr basic_state(position_type const& p, color c) noexcept
        :
                m_position(p),
                m_mrpk(),
                m_color(c)
        {
                assert_invariants();
        }

        bool operator==(basic_state const&) const = default;

        [[nodiscard]] static constexpr auto initial(int separation = initial_position_gap_v<Rules> + Board::height % 2) noexcept
                -> basic_state
        {
                return
                {
                        {
                                mask_type::initial(black_c, separation),
                                mask_type::initial(white_c, separation)
                        },
                        color::white
                };
        }

        [[nodiscard]] constexpr auto make(auto const& a) noexcept
        {
                m_position.make(m_color, a);
                pass_turn();
                assert_invariants();
        }

        [[nodiscard]] constexpr auto make(nullmove_t) noexcept
        {
                pass_turn();
                assert_invariants();
        }

        [[nodiscard]] constexpr auto pieces(auto&&... args) const noexcept
                requires (sizeof...(args) <= 2)
        {
                return m_position.pieces(std::forward<decltype(args)>(args)...);
        }

        [[nodiscard]] constexpr auto num_pieces(auto&&... args) const noexcept
                requires (sizeof...(args) <= 2)
        {
                return m_position.num_pieces(std::forward<decltype(args)>(args)...);
        }

        [[nodiscard]] constexpr auto targets(auto c, auto p) const noexcept
                requires xstd::is_integral_constant_v<decltype(c), color> && xstd::is_integral_constant_v<decltype(p), piece>
        {
                if constexpr (p == pawn_c && is_superior_rank_jump_v<Rules>) {
                        return pieces(!c, pawn_c);
                } else {
                        return pieces(!c);
                }
        }

        [[nodiscard]] constexpr auto to_move() const noexcept
        {
                return m_color;
        }
};

}       // namespace dctl::core
