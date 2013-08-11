#pragma once
#include <dctl/board/edge.hpp>          // ColumnLessEdge

namespace dctl {
namespace board {

// primary template declaration
template<class Dimensions, class Edge>
struct Grid;

// partial template declaration
template<class Dimensions>
struct Grid<Dimensions, ColumnLessEdge>;

}       // namespace board
}       // namespace dctl
