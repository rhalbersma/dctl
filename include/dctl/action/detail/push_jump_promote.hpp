#pragma once
#include <dctl/piece.hpp>
#include <dctl/utility/logic.hpp>
#include <dctl/utility/type_traits.hpp> // set_t
#include <xstd/cstdint.hpp>             // uint_fast
#include <cassert>                      // assert
#include <cstddef>                      // size_t

namespace dctl {
namespace detail {

template<class Rules, class Board>
class PushJumpPromote
{
public:
        using    set_type = set_t<Board>;
        using square_type = xstd::uint_fast_t<set_type::size()>;

private:
        set_type captured_;
        square_type from_;
        square_type dest_;
        Piece with_;
        Piece into_;

        constexpr auto invariant() const noexcept
        {
                return util::implies(from() == dest(), is_jump());
        }

public:
        PushJumpPromote() = default;

        constexpr PushJumpPromote(std::size_t const src, std::size_t const dst, bool const promotion) noexcept
        :
                captured_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::pawn},
                into_{promotion ? Piece::king : Piece::pawn}
        {
                assert(invariant());
        }

        constexpr PushJumpPromote(std::size_t const src, std::size_t const dst) noexcept
        :
                captured_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::king},
                into_{Piece::king}
        {
                assert(invariant());
        }

        auto capture_piece(std::size_t const sq) { captured_.set(sq); }
        auto release_piece(std::size_t const sq) { captured_.reset(sq); }
        auto set_from(std::size_t const src) { from_ = static_cast<square_type>(src); }
        auto set_dest(std::size_t const dst) { dest_ = static_cast<square_type>(dst); }
        auto set_with(Piece const p) { with_ = p; }
        auto set_into(Piece const p) { into_ = p; }

        constexpr auto captured() const noexcept
        {
                return captured_;
        }

        constexpr auto from() const noexcept
        {
                return static_cast<std::size_t>(from_);
        }

        constexpr auto dest() const noexcept
        {
                return static_cast<std::size_t>(dest_);
        }

        constexpr auto with() const noexcept
        {
                return with_;
        }

        constexpr auto into() const noexcept
        {
                return into_;
        }

        constexpr auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        constexpr auto is_into(Piece p) const noexcept
        {
                return into() == p;
        }

        constexpr auto is_jump() const noexcept
        {
                return captured().any();
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_with(Piece::pawn) && !is_into(Piece::pawn);
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with(Piece::king) && !is_jump();
        }

        constexpr auto num_captured() const noexcept
        {
                return captured().count();
        }

        constexpr auto is_with_king() const noexcept
        {
                return is_with(Piece::king);
        }
};

}       // namespace detail
}       // namespace dctl
