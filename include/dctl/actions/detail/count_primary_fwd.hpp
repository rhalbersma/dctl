#pragma once
#include <dctl/color.hpp>       // Color
#include <dctl/piece.hpp>       // Piece

namespace dctl {
namespace core {
namespace detail {

template<Color, Piece, class, class...>
class Count;

}       // namespace detail
}       // namespace core
}       // namespace dctl
