#pragma once
#include "../utility/Lazy.hpp"

namespace dctl {
namespace board {

LAZY_UNARY_METAFUNCTION(is_orthogonal)
LAZY_UNARY_METAFUNCTION(is_diagonal)
LAZY_UNARY_METAFUNCTION(is_up)
LAZY_UNARY_METAFUNCTION(is_down)
LAZY_UNARY_METAFUNCTION(is_left)
LAZY_UNARY_METAFUNCTION(is_right)
LAZY_UNARY_METAFUNCTION(is_positive)
LAZY_UNARY_METAFUNCTION(is_negative)

}       // namespace board
}       // namespace dctl
