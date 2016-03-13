#pragma once
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>      // set_type
#include <dctl/utility/logic.hpp>
#include <xstd/cstdint.hpp>
#include <cassert>                      // assert
#include <cstddef>                      // size_t

namespace dctl {
namespace detail {

template<class Rules, class Board>
class PushJumpPromote
{
public:
        using    set_type = get_set_type<Board>;
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

        // pawn push
        template<class... State>
        constexpr PushJumpPromote(std::size_t src, std::size_t dst, bool promotion, State const&...) noexcept
        :
                captured_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::pawn},
                into_{promotion ? Piece::king : Piece::pawn}
        {
                assert(invariant());
        }

        // king push
        template<class... State>
        constexpr PushJumpPromote(std::size_t src, std::size_t dst, State const&...) noexcept
        :
                captured_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::king},
                into_{Piece::king}
        {
                assert(invariant());
        }

        template<class State>
        PushJumpPromote(State const&)
        :
                captured_{},
                from_{},
                dest_{},
                with_{Piece::pawn},
                into_{Piece::king}
        {}

        template<class... State> auto pawn_jump_depart(std::size_t src, State const&...) { from_ = static_cast<square_type>(src); }
        template<class... State> auto pawn_jump_arrive(std::size_t dst, State const&...) { dest_ = static_cast<square_type>(dst); }
        template<class... State> auto king_jump_depart(std::size_t src, State const&...) { from_ = static_cast<square_type>(src); }
        template<class... State> auto king_jump_arrive(std::size_t dst, State const&...) { dest_ = static_cast<square_type>(dst); }
        template<class... State> auto capture(std::size_t sq, State const&...) { captured_.set(sq); }
        auto promote(std::size_t) { into_ = Piece::king; }
        template<class... State> auto king_captures(set_type const&, State const&...) {}

        template<class... State>
        constexpr auto captured(State const&...) const noexcept
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

        constexpr auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        template<class... State>
        constexpr auto is_with_king(State const&...) const noexcept
        {
                return is_with(Piece::king);
        }

        constexpr auto into() const noexcept
        {
                return into_;
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

        template<class... State>
        constexpr auto num_captured(State const&...) const noexcept
        {
                return captured().count();
        }
};

}       // namespace detail
}       // namespace dctl
