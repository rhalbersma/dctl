#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type

namespace dctl {
namespace detail {
namespace wma {
namespace block_adl {

template<class Board>
class base_state
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type white_;
        set_type pawns_;
        set_type pieces_;

public:
        base_state() = default;

        base_state(set_type const b, set_type const w, set_type const p, set_type const /* k */)
        :
                white_{w},
                pawns_{p},
                pieces_{b | w}
        {}

        template<class Action>
        auto& make(color const c, Action const& a)
        {
                pieces_.reset(a.from());
                pieces_.set  (a.dest());

                if (c == color::white) {
                        white_.reset(a.from());
                        white_.set  (a.dest());
                }

                if (a.with() == piece::pawn)
                        pawns_.reset(a.from());

                if (a.into() == piece::pawn)
                        pawns_.set(a.dest());

                if (a.is_jump()) {
                        pieces_ ^= a.captured_pieces();
                        white_ &= ~a.captured_pieces();
                        pawns_ &= ~a.captured_pieces();
                }

                return *this;
        }

        auto pieces(color const c) const noexcept
        {
                return c == color::black ? pieces_ ^ white_ : white_;
        }

        auto pieces(piece const p) const noexcept
        {
                return p == piece::pawn ? pawns_ : pieces_ ^ pawns_;
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return c == color::black ?
                        (p == piece::pawn ?  ~white_ & pawns_ : pieces_ ^ (white_ | pawns_)) :
                        (p == piece::pawn ?   white_ & pawns_ : white_ & ~pawns_)
                ;
        }

        auto pieces() const noexcept
        {
                return pieces_;
        }

        auto not_occupied() const noexcept
        {
                return mask::squares_v<Board> ^ pieces();
        }

        auto num_pieces(color const c, piece const p) const noexcept
        {
                return pieces(c, p).count();
        }
};

}       // namespace block_adl

using block_adl::base_state;

}       // namespace wma
}       // namespace detail
}       // namespace dctl
