#pragma once
#include <dctl/board_traits.hpp>
#include <dctl/color_piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/state/position.hpp>
#include <dctl/utility/conditional_base.hpp>    // conditional_base
#include <dctl/utility/type_traits.hpp>         // set_t
#include <hash_append/hash_append.h>            // hash_append
#include <cassert>                              // assert
#include <cstdint>                              // uint64_t
#include <type_traits>                          // is_pod, is_same
#include <tuple>                                // tie
#include <utility>                              // forward

namespace dctl {
namespace detail {
namespace block_adl {

template<class Board>
struct base_position
{
        using position_type = wpo::position<Board>;
        position_type m_position;
};

struct base_color
{
        color m_color;
};

template<class Board>
struct most_recently_pushed_kings {};

template<class Rules, class Board>
using conditional_base_mrpk = util::conditional_base<
        is_restricted_king_push_v<Rules>,
        most_recently_pushed_kings<Board>
>;

}       // namespace block_adl

using block_adl::base_position;
using block_adl::base_color;
using block_adl::conditional_base_mrpk;

}       // namespace detail

template<class Rules, class Board = rectangular_t<Rules>>
class state
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
        using      set_type = set_t<Board>;
        using position_type = position_t<base_position>;

private:
        static constexpr auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod<state>{});
        }

        constexpr auto assert_invariants() const noexcept
        {
                assert(true);
        }

public:
        state(position_type const& position, color const to_move)
        :
                base_position{position},
                conditional_base_mrpk{},
                base_color{to_move}
        {
                assert_invariants();
        }

        static constexpr auto initial()
                -> state
        {
                constexpr auto separation = initial_position_gap_v<Rules> + Board::height % 2;
                constexpr auto black_pawns = initial_v<board_type, black_>(separation);
                constexpr auto white_pawns = initial_v<board_type, white_>(separation);
                return {{black_pawns, white_pawns}, color::white};
        }

        static constexpr auto initial(int const separation)
                -> state
        {
                auto const black_pawns = initial_v<board_type, black_>(separation);
                auto const white_pawns = initial_v<board_type, white_>(separation);
                return {{black_pawns, white_pawns}, color::white};
        }

        template<class Action>
        auto make(Action const& a)
        {
                static_assert(std::is_same<rules_type, rules_t<Action>>{});
                static_assert(std::is_same<board_type, board_t<Action>>{});
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
                static_assert(std::is_same<rules_type, rules_t<Action>>{});
                static_assert(std::is_same<board_type, board_t<Action>>{});
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
        auto pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return this->m_position.pieces(std::forward<Args>(args)...);
        }

        template<color Side, piece Type>
        auto targets(color_<Side>, piece_<Type>) const noexcept
        {
                static constexpr auto not_to_move_c = !color_c<Side>;
                if constexpr (Type == piece::pawns && is_superior_rank_jump_v<rules_type>) {
                        return pieces(color_c<!Side>, pawns_c);
                } else {
                        return pieces(not_to_move_c);
                }
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return this->m_position.num_pieces(std::forward<Args>(args)...);
        }

        auto to_move() const noexcept
        {
                return m_color;
        }

        auto tied() const
        {
                return std::tie(this->m_position, m_color);
        }

        template<class HashAlgorithm>
        friend auto hash_append(HashAlgorithm& h, state const& s)
        {
                using xstd::hash_append;
                hash_append(h, s.m_position, s.m_color);
        }

private:
        constexpr auto pass_turn() noexcept
        {
                m_color = !m_color;
        }
};

template<class Rules, class Board>
constexpr auto operator==(state<Rules, Board> const& lhs, state<Rules, Board> const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
}

template<class Rules, class Board>
constexpr auto operator< (state<Rules, Board> const& lhs, state<Rules, Board> const& rhs) noexcept
{
        return lhs.tied() < rhs.tied();
}

template<class Rules, class Board>
constexpr auto operator!=(state<Rules, Board> const& lhs, state<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Rules, class Board>
constexpr auto operator> (state<Rules, Board> const& lhs, state<Rules, Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Rules, class Board>
constexpr auto operator>=(state<Rules, Board> const& lhs, state<Rules, Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Rules, class Board>
constexpr auto operator<=(state<Rules, Board> const& lhs, state<Rules, Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

}       // namespace dctl
