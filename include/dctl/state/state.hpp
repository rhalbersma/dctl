#pragma once
#include <dctl/action/action.hpp>
#include <dctl/board/mask/initial.hpp>
#include <dctl/board/mask/promotion.hpp>
#include <dctl/board/mask/squares.hpp>
#include <dctl/color_piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/state/mrp_kings/mrp_kings.hpp>
#include <dctl/state/mrp_kings/zobrist.hpp>
#include <dctl/state/position.hpp>
#include <dctl/state/to_move/to_move.hpp>
#include <dctl/state/to_move/zobrist.hpp>
#include <dctl/state/reversible_actions.hpp>
#include <dctl/utility/concepts.hpp>            // is_trivial_special_members
#include <dctl/utility/conditional_base.hpp>    // conditional_base
#include <dctl/utility/type_traits.hpp>         // set_t
#include <dctl/utility/zobrist/accumulate.hpp>
#include <xstd/bitset.hpp>
#include <experimental/optional>
#include <cassert>                              // assert
#include <type_traits>                          // is_same
#include <tuple>
#include <utility>                              // forward

namespace dctl {
namespace detail {
namespace block_adl {

template<class Board>
struct base_position
{
        wpo::position<Board> m_position;
};

struct base_color
{
        color m_to_move;
};

template<class Board>
struct most_recently_pushed_kings
{
        std::experimental::optional<square_t<Board>> kings_[2];
        square_t<Board> moves_[2];
};

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
        using rules_type = Rules;
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        static constexpr auto static_assert_type_traits() noexcept
        {
                using T = state<Rules, Board>;
                static_assert(util::is_trivial_special_members_v<T>);
                static_assert(std::is_pod<T>{});
        }

        constexpr auto assert_invariants() const noexcept
        {
                assert(board::mask::squares_v<board_type> == (pieces(occup_c) | pieces(empty_c)));

                assert(pieces(occup_c) == (pieces(black_c) | pieces(white_c)));
                assert(pieces(occup_c) == (pieces(pawns_c) | pieces(kings_c)));

                assert(pieces(black_c) == (pieces(black_c, pawns_c) | pieces(black_c, kings_c)));
                assert(pieces(white_c) == (pieces(white_c, pawns_c) | pieces(white_c, kings_c)));
                assert(pieces(pawns_c) == (pieces(black_c, pawns_c) | pieces(white_c, pawns_c)));
                assert(pieces(kings_c) == (pieces(black_c, kings_c) | pieces(white_c, kings_c)));

                assert(xstd::disjoint(pieces(occup_c), pieces(empty_c)));

                assert(xstd::disjoint(pieces(black_c), pieces(white_c)));
                assert(xstd::disjoint(pieces(pawns_c), pieces(kings_c)));

                assert(xstd::disjoint(pieces(black_c, pawns_c), pieces(black_c, kings_c)));
                assert(xstd::disjoint(pieces(white_c, pawns_c), pieces(white_c, kings_c)));
                assert(xstd::disjoint(pieces(black_c, pawns_c), pieces(white_c, pawns_c)));
                assert(xstd::disjoint(pieces(black_c, kings_c), pieces(white_c, kings_c)));

                assert(xstd::disjoint(pieces(black_c, pawns_c), board::mask::promotion_v<board_type, black_>));
                assert(xstd::disjoint(pieces(white_c, pawns_c), board::mask::promotion_v<board_type, white_>));
        }

public:
        state(color const to_move, set_type const black, set_type const white, set_type const pawns, set_type const kings)
        :
                base_position{{black, white, pawns, kings}},
                conditional_base_mrpk{},
                base_color{to_move}
        {
                assert_invariants();
        }

        static state initial(std::size_t const separation = initial_position_gap_v<Rules> + Board::height % 2)
        {
                auto const bp = board::mask::initial<board_type>{}(color::black, separation);
                auto const wp = board::mask::initial<board_type>{}(color::white, separation);
                return { color::white, bp, wp, bp | wp, {} };
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
                if constexpr (Type == piece::pawn && is_superior_rank_jump_v<rules_type>) {
                        return pieces(color_c<!Side>, pawns_c);
                } else {
                        return pieces(not_to_move_c);
                }
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).count();
        }

        auto to_move() const noexcept
        {
                return m_to_move;
        }

        auto hash() const
        {
                return std::size_t{0};
        }

private:
        constexpr auto pass_turn() noexcept
        {
                m_to_move = !m_to_move;
        }
};

}       // namespace dctl
