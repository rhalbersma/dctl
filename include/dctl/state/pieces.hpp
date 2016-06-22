#pragma once
#include <dctl/color.hpp>       // color
#include <dctl/piece.hpp>       // piece

namespace dctl {

template<color Side, class State>
constexpr auto pieces(State const& s) noexcept
{
        return s.pieces(Side);
}

template<piece Type, class State>
constexpr auto pieces(State const& s) noexcept
{
        return s.pieces(Type);
}

template<color Side, piece Type, class State>
constexpr auto pieces(State const& s) noexcept
{
        return s.pieces(Side, Type);
}

}       // namespace dctl
