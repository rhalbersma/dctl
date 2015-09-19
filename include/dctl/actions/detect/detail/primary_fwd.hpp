#pragma once
#include <dctl/color.hpp>       // Color
#include <dctl/piece.hpp>       // Piece

namespace dctl {
namespace actions {
namespace detail {

template<Color, Piece, class, class...>
class Detect;

}       // namespace detail
}       // namespace actions
}       // namespace dctl
