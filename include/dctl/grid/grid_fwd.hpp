#pragma once

namespace dctl {
namespace grid {

// primary template declaration
template<class Dimensions, int EdgeColumns>
class Grid;

// partial template specialization declaration
template<class Dimensions>
class Grid<Dimensions, 0>;

}       // namespace grid
}       // namespace dctl
