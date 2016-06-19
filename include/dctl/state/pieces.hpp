#pragma once
#include <dctl/color.hpp>       // Color
#include <dctl/piece.hpp>       // Piece

namespace dctl {

template<Color Side, class State>
constexpr auto pieces(State const& s) noexcept
{
        return s.pieces(Side);
}

template<Piece Type, class State>
constexpr auto pieces(State const& s) noexcept
{
        return s.pieces(Type);
}

template<Color Side, Piece Type, class State>
constexpr auto pieces(State const& s) noexcept
{
        return s.pieces(Side, Type);
}

}       // namespace dctl
