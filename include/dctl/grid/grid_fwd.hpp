#pragma once
#include <dctl/grid/edge.hpp>          // ColumnLessEdge

namespace dctl {
namespace grid {

// primary template declaration
template<class Dimensions, class Edge>
struct Grid;

// partial template specialization declaration
template<class Dimensions>
struct Grid<Dimensions, ColumnLessEdge>;

}       // namespace grid
}       // namespace dctl
