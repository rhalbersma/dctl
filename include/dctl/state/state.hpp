#pragma once
#include <dctl/action/action.hpp>
#include <dctl/board/mask/initial.hpp>
#include <dctl/board/mask/promotion.hpp>
#include <dctl/board/mask/squares.hpp>
#include <dctl/color_piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/state/mrp_kings/mrp_kings.hpp>
#include <dctl/state/mrp_kings/zobrist.hpp>
#include <dctl/state/base_state.hpp>
#include <dctl/state/player_to_move.hpp>
#include <dctl/state/to_move/to_move.hpp>
#include <dctl/state/to_move/zobrist.hpp>
#include <dctl/state/reversible_actions.hpp>
#include <dctl/utility/tagged_empty_base.hpp>
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
struct most_recently_pushed_kings
{
        std::experimental::optional<square_t<Board>> kings_[2];
        square_t<Board> moves_[2];
};

template<class Rules, class Board>
using most_recently_pushed_kings_t = std::conditional_t<
        is_restricted_king_push_v<Rules>,
        most_recently_pushed_kings<Board>,
        util::tagged_empty_base<0>
>;

}       // namespace block_adl

using block_adl::most_recently_pushed_kings_t;

}       // namespace detail

template<class Rules, class Board = rectangular_t<Rules>>
class state
:
        detail::most_recently_pushed_kings_t<Rules, Board>
{
        using most_recently_pushed_kings_t = detail::most_recently_pushed_kings_t<Rules, Board>;
public:
        using rules_type = Rules;
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        PlayerToMove player_to_move_{};
        detail::wma::BaseState<board_type> placement_{};

        constexpr auto assert_invariants() const noexcept
        {
                assert(board::mask::squares_v<board_type> == (pieces() | pieces(none_type{})));

                assert(pieces() == (pieces(black_type{}) | pieces(white_type{})));
                assert(pieces() == (pieces( pawn_type{}) | pieces( king_type{})));

                assert(pieces(black_type{}) == (pieces(black_type{}, pawn_type{}) | pieces(black_type{}, king_type{})));
                assert(pieces(white_type{}) == (pieces(white_type{}, pawn_type{}) | pieces(white_type{}, king_type{})));
                assert(pieces( pawn_type{}) == (pieces(black_type{}, pawn_type{}) | pieces(white_type{}, pawn_type{})));
                assert(pieces( king_type{}) == (pieces(black_type{}, king_type{}) | pieces(white_type{}, king_type{})));

                assert(xstd::disjoint(pieces(), pieces(none_type{})));

                assert(xstd::disjoint(pieces(black_type{}), pieces(white_type{})));
                assert(xstd::disjoint(pieces( pawn_type{}), pieces( king_type{})));

                assert(xstd::disjoint(pieces(black_type{}, pawn_type{}), pieces(black_type{}, king_type{})));
                assert(xstd::disjoint(pieces(white_type{}, pawn_type{}), pieces(white_type{}, king_type{})));
                assert(xstd::disjoint(pieces(black_type{}, pawn_type{}), pieces(white_type{}, pawn_type{})));
                assert(xstd::disjoint(pieces(black_type{}, king_type{}), pieces(white_type{}, king_type{})));

                assert(xstd::disjoint(pieces(black_type{}, pawn_type{}), board::mask::promotion_v<board_type, black_type>));
                assert(xstd::disjoint(pieces(white_type{}, pawn_type{}), board::mask::promotion_v<board_type, white_type>));
        }

public:
        state(Color const c, set_type const black, set_type const white, set_type const pawns, set_type const kings)
        :
                most_recently_pushed_kings_t{},
                player_to_move_{c},
                placement_{black, white, pawns, kings}
        {
                assert_invariants();
        }

        static state initial(std::size_t const separation = initial_position_gap_v<Rules> + Board::height % 2)
        {
                auto const bp = board::mask::initial<board_type>{}(Color::black, separation);
                auto const wp = board::mask::initial<board_type>{}(Color::white, separation);
                return { Color::white, bp, wp, bp | wp, {} };
        }

        template<class Action>
        auto& make(Action const& a)
        {
                static_assert(std::is_same<rules_type, rules_t<Action>>{});
                static_assert(std::is_same<board_type, board_t<Action>>{});
                placement_.make(player_to_move_, a);
                player_to_move_.make(a);
                assert_invariants();
                return *this;
        }

        template<class... Args>
        auto pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return placement_.pieces(std::forward<Args>(args)...);
        }

        auto king_targets(Color const c) const noexcept
        {
                return pieces(!c);
        }

        template<Color Side>
        auto king_targets(color_constant<Side>) const noexcept
        {
                return king_targets(Side);
        }

        auto pawn_targets(Color const c) const noexcept
        {
                if constexpr (is_superior_rank_jump_v<rules_type>) {
                        return pieces(!c, pawn_type{});
                } else {
                        return pieces(!c);
                }
        }

        template<Color Side>
        auto pawn_targets(color_constant<Side>) const noexcept
        {
                return pawn_targets(Side);
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).count();
        }

        auto to_move() const noexcept
        {
                return player_to_move_;
        }

        auto is_to_move(Color const c) const noexcept
        {
                return to_move() == c;
        }

        auto hash() const
        {
                return std::size_t{0};
        }
};

}       // namespace dctl
