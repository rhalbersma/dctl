#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>            // set_type
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
        Color to_move_;

        constexpr auto invariant() const noexcept
        {
                return util::implies(from() == dest(), is_jump());
        }

public:
        PushJumpPromote() = default;

        // pawn push
        constexpr PushJumpPromote(std::size_t src, std::size_t dst, Piece promotion, Color c) noexcept
        :
                captured_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::pawn},
                into_{promotion},
                to_move_{c}
        {
                assert(invariant());
        }

        // king push
        constexpr PushJumpPromote(std::size_t src, std::size_t dst, Color color) noexcept
        :
                captured_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::king},
                into_{Piece::king},
                to_move_{color}
        {
                assert(invariant());
        }

        // jump
        template<class Tracker>
        explicit constexpr PushJumpPromote(Tracker const& t)
        :
                captured_{t.captured()},
                from_{static_cast<square_type>(t.from())},
                dest_{static_cast<square_type>(t.dest())},
                with_{t.with()},
                into_{t.into()},
                to_move_{t.to_move()}
        {
                assert(invariant());
        }

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

        constexpr auto is_with(Piece p) const noexcept
        {
                return with() == p;
        }

        constexpr auto into() const noexcept
        {
                return into_;
        }

        constexpr auto is_into(Piece p) const noexcept
        {
                return into() == p;
        }

        constexpr auto to_move() const noexcept
        {
                return to_move_;
        }

        constexpr auto is_to_move(Color c) const noexcept
        {
                return to_move() == c;
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
};

}       // namespace detail
}       // namespace dctl
