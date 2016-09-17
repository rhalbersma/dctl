#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type

namespace dctl {
namespace detail {
namespace c2p2e {
namespace block_adl {

template<class Board>
class base_state
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type by_Color_[2];
        set_type by_Piece_[2];
        set_type not_occupied_;

public:
        base_state() = default;

        base_state(set_type const b, set_type const w, set_type const p, set_type const k)
        :
                by_Color_{b, w},
                by_Piece_{p, k},
                not_occupied_{mask::squares_v<Board> ^ (b | w)}
        {}

        template<class Action>
        auto& make(Color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());
                pieces(a.with()).reset(a.from());
                pieces(a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        pieces(Piece::pawn ) &= ~a.captured_pieces();
                        pieces(Piece::king ) &= ~a.captured_pieces();
                }

                not_occupied_ = mask::squares_v<Board> ^ (pieces(Color::black) | pieces(Color::white));

                return *this;
        }

        auto pieces(Color const c) const noexcept
        {
                return by_Color_[xstd::to_underlying_type(c)];
        }

        auto pieces(Piece const p) const noexcept
        {
                return by_Piece_[xstd::to_underlying_type(p)];
        }

        auto pieces(Color const c, Piece const p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces() const noexcept
        {
                return mask::squares_v<Board> ^ not_occupied();
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
        auto& pieces(Color const c) noexcept
        {
                return by_Color_[xstd::to_underlying_type(c)];
        }

        auto& pieces(Piece const p) noexcept
        {
                return by_Piece_[xstd::to_underlying_type(p)];
        }
};

}       // namespace block_adl

using block_adl::base_state;

}       // namespace c2p2e
}       // namespace detail
}       // namespace dctl
