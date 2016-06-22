#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type

namespace dctl {
namespace detail {
namespace bwke {
namespace block_adl {

template<class Board>
class base_state
{
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

private:
        set_type by_color_[2];
        set_type kings_;
        set_type not_occupied_;

public:
        base_state() = default;

        base_state(set_type const b, set_type const w, set_type const /* p */, set_type const k)
        :
                by_color_{b, w},
                kings_{k},
                not_occupied_{board::squares_v<Board> ^ (b | w)}
        {}

        template<class Action>
        auto& make(color const c, Action const& a)
        {
                pieces(c).reset(a.from());
                pieces(c).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!c) ^= a.captured_pieces();
                        kings_ &= ~a.captured_pieces();
                        not_occupied_ ^= a.captured_pieces();
                }

                if (a.with() == piece::king) {
                        kings_.reset(a.from());
                        kings_.set(a.dest());
                } else if (a.into() == piece::king) {
                        kings_.set(a.dest());
                }

                not_occupied_.set  (a.from());
                not_occupied_.reset(a.dest());

                return *this;
        }

        auto pieces(color const c) const noexcept
        {
                return by_color_[xstd::to_underlying_type(c)];
        }

        auto pieces(piece const p) const noexcept
        {
                return p == piece::pawn ? pieces() ^ kings_ : kings_;
        }

        auto pieces(color const c, piece const p) const noexcept
        {
                return pieces(c) & (p == piece::pawn ? ~kings_ : kings_);
        }

        auto pieces() const noexcept
        {
                return board::squares_v<Board> ^ not_occupied();
        }

        auto not_occupied() const noexcept
        {
                return not_occupied_;
        }

        auto num_pieces(color const c, piece const p) const noexcept
        {
                return pieces(c, p).count();
        }

private:
        auto& pieces(color const c) noexcept
        {
                return by_color_[xstd::to_underlying_type(c)];
        }
};

}       // namespace block_adl

using block_adl::base_state;

}       // namespace bwke
}       // namespace detail
}       // namespace dctl
