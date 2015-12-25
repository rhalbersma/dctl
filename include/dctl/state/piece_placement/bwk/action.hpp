#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/board/set_type.hpp>
#include <dctl/state/piece_placement/invariant.hpp>
#include <xstd/cstdint.hpp>
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert
#include <tuple>

namespace dctl {
namespace detail {
namespace bwk {

template<class Rules, class Board>
class Action
{
public:
        using board_type = Board;
        using set_type = get_set_type<Board>;
        using square_type = xstd::uint_fast_t<set_type::size()>;
private:
        set_type by_color[2];
        set_type kings_;
        Color to_move_;

public:
        Action() = default;

        Action(std::size_t src, std::size_t dst, bool promotion, Color to_move)
        :
                Action{create(set_type{src, dst}, set_type{}, promotion ? set_type{dst} : set_type{}, to_move)}
        {}

        Action(std::size_t src, std::size_t dst, Color to_move)
        :
                Action{create(set_type{src, dst}, set_type{}, set_type{src, dst}, to_move)}
        {}

        template<class Builder>
        Action(Builder const& b)
        :
                Action{create(
                        b.from() == b.dest() ? set_type{} : set_type{b.from(), b.dest()},
                        b.captured(),
                        b.captured_kings() ^
                        (b.is_promotion() ? set_type{b.dest()} : set_type{}) ^
                        ((b.is_with_king() && (b.from() != b.dest())) ? set_type{b.from(), b.dest()} : set_type{}),
                        b.to_move()
                )}
        {}

        auto pieces(Color c) const noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto kings() const noexcept
        {
                return kings_;
        }

        auto num_captured() const noexcept
        {
                return pieces(!to_move_).count();
        }

private:
        auto init(set_type pushed_pieces, set_type jumped_pieces, set_type promoted_piece_and_jumped_kings, Color to_move) noexcept
        {
                pieces( to_move) = pushed_pieces;
                pieces(!to_move) = jumped_pieces;
                kings_ = promoted_piece_and_jumped_kings;
                to_move_ = to_move;
        }

        static auto create(set_type pushed_pieces, set_type jumped_pieces, set_type promoted_piece_and_jumped_kings, Color to_move)
        {
                Action nrv;
                nrv.init(pushed_pieces, jumped_pieces, promoted_piece_and_jumped_kings, to_move);
                return nrv;
        }

        auto& pieces(Color c) noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }
};

template<class Rules, class Board>
auto as_tuple(Action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.pieces(Color::black), a.pieces(Color::white), a.kings());
}

template<class Rules, class Board>
constexpr auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) == as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator< (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) < as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator!=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Rules, class Board>
constexpr auto operator> (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Rules, class Board>
constexpr auto operator>=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Rules, class Board>
constexpr auto operator<=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

}       // namespace bwk
}       // namespace detail
}       // namespace dctl
