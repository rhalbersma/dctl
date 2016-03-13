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

public:
        Action() = default;

        template<class State>
        Action(std::size_t src, std::size_t dst, bool promotion, State const& state)
        :
                Action{create(set_type{src, dst}, set_type{}, promotion ? set_type{dst} : set_type{}, state)}
        {}

        template<class State>
        Action(std::size_t src, std::size_t dst, State const& state)
        :
                Action{create(set_type{src, dst}, set_type{}, set_type{src, dst}, state)}
        {}

        template<class State>
        Action(State const&)
        :
               by_color{},
               kings_{}
        {}

        template<class State>
        auto pawn_jump_depart(std::size_t src, State const& state)
        {
                pieces(state.to_move()).flip(src);
        }

        template<class State>
        auto pawn_jump_arrive(std::size_t dst, State const& state)
        {
                pieces(state.to_move()).flip(dst);
        }

        template<class State>
        auto king_jump_depart(std::size_t src, State const& state)
        {
                pieces(state.to_move()).flip(src);
                kings_.flip(src);
        }

        template<class State>
        auto king_jump_arrive(std::size_t dst, State const& state)
        {
                pieces(state.to_move()).flip(dst);
                kings_.flip(dst);
        }

        template<class State>
        auto capture(std::size_t sq, State const& state)
        {
                pieces(!state.to_move()).flip(sq);
        }

        auto promote(std::size_t dst)
        {
                kings_.flip(dst);
        }

        template<class State>
        auto king_captures(set_type const& k, State const& state)
        {
                kings_ ^= captured(state) & k;
        }

        auto pieces(Color c) const noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto kings() const noexcept
        {
                return kings_;
        }

        template<class State>
        auto captured(State const& state) const noexcept
        {
                return pieces(!state.to_move());
        }

        template<class State>
        auto num_captured(State const& state) const noexcept
        {
                return captured(state).count();
        }

private:
        template<class State>
        auto init(set_type pushed_pieces, set_type jumped_pieces, set_type promoted_piece_and_jumped_kings, State const& state) noexcept
        {
                pieces( state.to_move()) = pushed_pieces;
                pieces(!state.to_move()) = jumped_pieces;
                kings_ = promoted_piece_and_jumped_kings;
        }

        template<class State>
        static auto create(set_type pushed_pieces, set_type jumped_pieces, set_type promoted_piece_and_jumped_kings, State const& state)
        {
                Action nrv;
                nrv.init(pushed_pieces, jumped_pieces, promoted_piece_and_jumped_kings, state);
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
        return std::make_tuple(
                a.pieces(Color::black),
                a.pieces(Color::white),
                a.kings()
        );
}

template<class Rules, class Board>
constexpr auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) == as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator!=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

}       // namespace bwk
}       // namespace detail
}       // namespace dctl
