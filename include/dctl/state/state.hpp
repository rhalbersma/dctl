#pragma once
#include <dctl/action/action.hpp>
#include <dctl/color.hpp>
#include <dctl/mask/initial.hpp>
#include <dctl/mask/promotion.hpp>
#include <dctl/mask/squares.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/state/mrp_kings/mrp_kings.hpp>
#include <dctl/state/mrp_kings/zobrist.hpp>
#include <dctl/state/detail/base_state.hpp>
#include <dctl/state/pieces.hpp>
#include <dctl/state/player_to_move.hpp>
#include <dctl/state/to_move/to_move.hpp>
#include <dctl/state/to_move/zobrist.hpp>
#include <dctl/state/reversible_actions.hpp>
#include <dctl/utility/tagged_empty_base.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <dctl/utility/zobrist/accumulate.hpp>
#include <xstd/bitset.hpp>
#include <experimental/optional>
#include <experimental/type_traits>
#include <cassert>                              // assert
#include <type_traits>
#include <tuple>

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
using most_recently_pushed_kings_or_t = std::conditional_t<
        is_restricted_king_push_v<Rules>,
        most_recently_pushed_kings<Board>,
        util::tagged_empty_base<0>
>;

}       // namespace block_adl

using block_adl::most_recently_pushed_kings_or_t;

}       // namespace detail

template<class Rules, class Board = rectangular_t<Rules>>
class state
:
        detail::most_recently_pushed_kings_or_t<Rules, Board>
{
        using most_recently_pushed_kings_or_t = detail::most_recently_pushed_kings_or_t<Rules, Board>;
public:
        using rules_type = Rules;
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        PlayerToMove player_to_move_{};
        detail::wma::base_state<board_type> piece_placement_{};

        constexpr auto assert_invariants() const noexcept
        {
                assert(mask::squares_v<board_type> == (pieces() | not_occupied()));

                assert(pieces() == (pieces(color::black) | pieces(color::white)));
                assert(pieces() == (pieces(piece::pawn ) | pieces(piece::king )));

                assert(pieces(color::black) == (pieces(color::black, piece::pawn) | pieces(color::black, piece::king)));
                assert(pieces(color::white) == (pieces(color::white, piece::pawn) | pieces(color::white, piece::king)));
                assert(pieces(piece::pawn ) == (pieces(color::black, piece::pawn) | pieces(color::white, piece::pawn)));
                assert(pieces(piece::king ) == (pieces(color::black, piece::king) | pieces(color::white, piece::king)));

                assert(xstd::disjoint(pieces(), not_occupied()));

                assert(xstd::disjoint(pieces(color::black), pieces(color::white)));
                assert(xstd::disjoint(pieces(piece::pawn ), pieces(piece::king )));

                assert(xstd::disjoint(pieces(color::black, piece::pawn), pieces(color::black, piece::king)));
                assert(xstd::disjoint(pieces(color::white, piece::pawn), pieces(color::white, piece::king)));
                assert(xstd::disjoint(pieces(color::black, piece::pawn), pieces(color::white, piece::pawn)));
                assert(xstd::disjoint(pieces(color::black, piece::king), pieces(color::white, piece::king)));

                assert(xstd::disjoint(pieces(color::black, piece::pawn), mask::promotion_v<board_type, color::black>));
                assert(xstd::disjoint(pieces(color::white, piece::pawn), mask::promotion_v<board_type, color::white>));
        }

public:
        state(color const c, set_type const black, set_type const white, set_type const pawns, set_type const kings)
        :
                most_recently_pushed_kings_or_t{},
                player_to_move_{c},
                piece_placement_{black, white, pawns, kings}
        {
                assert_invariants();
        }

        static state initial(std::size_t const separation = initial_position_gap_or_v<Rules> + Board::height % 2)
        {
                auto const bp = mask::initial<board_type>{}(color::black, separation);
                auto const wp = mask::initial<board_type>{}(color::white, separation);
                return { color::white, bp, wp, bp | wp, {} };
        }

        template<class Action>
        auto& make(Action const& a)
        {
                static_assert(std::experimental::is_same_v<rules_type, rules_t<Action>>);
                static_assert(std::experimental::is_same_v<board_type, board_t<Action>>);
                piece_placement_.make(player_to_move_, a);
                player_to_move_.make(a);
                assert_invariants();
                return *this;
        }

        // observers

        auto pieces(color const c) const noexcept
        {
                return piece_placement_.pieces(c);
        }

        auto pieces(piece const p) const noexcept
        {
                return piece_placement_.pieces(p);
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return piece_placement_.pieces(c, p);
        }

        auto pieces() const noexcept
        {
                return piece_placement_.pieces();
        }

        auto not_occupied() const noexcept
        {
                return piece_placement_.not_occupied();
        }

        auto king_targets(color const c) const noexcept
        {
                return pieces(c);
        }

        auto pawn_targets(color const c) const noexcept
        {
                return pawn_targets_dispatch(c, rank_jump_category_t<rules_type>{});
        }

        auto num_pieces(color const c, piece const p) const noexcept
        {
                return piece_placement_.num_pieces(c, p);
        }

        auto to_move() const noexcept
        {
                return player_to_move_;
        }

        auto is_to_move(color const c) const noexcept
        {
                return to_move() == c;
        }

        auto hash() const
        {
                return std::size_t{0};
        }
private:
        auto pawn_targets_dispatch(color const c, inferior_rank_jump_tag) const noexcept
        {
                return pieces(c);
        }

        auto pawn_targets_dispatch(color const c, superior_rank_jump_tag) const noexcept
        {
                return pieces(c, piece::pawn);
        }
};

}       // namespace dctl
