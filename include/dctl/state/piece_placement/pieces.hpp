#pragma once
#include <dctl/color.hpp>       // Color
#include <dctl/piece.hpp>       // Piece

namespace dctl {

template<Color Side, class PiecePlacement>
auto pieces(PiecePlacement const& p) noexcept
{
        return p.pieces(Side);
}

template<Piece Type, class PiecePlacement>
auto pieces(PiecePlacement const& p) noexcept
{
        return p.pieces(Type);
}

template<Color Side, Piece Type, class PiecePlacement>
auto pieces(PiecePlacement const& p) noexcept
{
        return p.pieces(Side, Type);
}

}       // namespace dctl
