#pragma once
#include <dctl/color.hpp>       // color
#include <dctl/piece.hpp>       // piece

namespace dctl {
namespace core {
namespace detail {

template<color, piece, class, class...>
class Detect;

}       // namespace detail
}       // namespace core
}       // namespace dctl
