#pragma once

namespace dctl {
namespace grid {

// primary template declaration
template<class Dimensions, int EdgeColumns>
class GridClass;

// partial template specialization declaration
template<class Dimensions>
class GridClass<Dimensions, 0>;

}       // namespace grid
}       // namespace dctl
