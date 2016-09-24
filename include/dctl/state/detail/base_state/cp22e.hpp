#pragma once
#include <dctl/color.hpp>               // black, white
#include <dctl/piece.hpp>               // pawn, king
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/bitset.hpp>              // disjoint
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <cassert>                      // assert

namespace dctl {
namespace detail {
namespace cp22e {
namespace block_adl {

template<class Board>
class base_state
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type by_color_piece[2][2];
        set_type not_occupied_;

public:
        base_state() = default;

        base_state(set_type const b, set_type const w, set_type const p, set_type const k)
        :
                by_color_piece{{b & p, b & k}, {w & p, w & k}},
                not_occupied_{mask::squares_v<board_type> ^ (b | w)}
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                pieces(c, a.with()).reset(a.from());
                pieces(c, a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c, pawn_type{}) &= ~a.captured_pieces();
                        pieces(!c, king_type{}) &= ~a.captured_pieces();
                        not_occupied_ ^= a.captured_pieces();
                }

                not_occupied_.set  (a.from());
                not_occupied_.reset(a.dest());

                return *this;
        }

        auto pieces(Color const c) const noexcept
        {
                return pieces(c, pawn_type{}) ^ pieces(c, king_type{});
        }

        auto pieces(Piece const p) const noexcept
        {
                return pieces(black_type{}, p) ^ pieces(white_type{}, p);
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return by_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        auto pieces() const noexcept
        {
                return mask::squares_v<board_type> ^ not_occupied();
        }

        auto not_occupied() const noexcept
        {
                return not_occupied_;
        }

        auto num_pieces(Color const c, Piece const p) const noexcept
        {
                return pieces(c, p).count();
        }

private:
        auto& pieces(Color const c, Piece const p) noexcept
        {
                return by_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }
};

}       // namespace block_adl

using block_adl::base_state;

}       // namespace cp22e
}       // namespace detail
}       // namespace dctl
