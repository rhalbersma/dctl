#pragma once
#include <dctl/angle/degrees.hpp>       // D00, D045, D090, D135, D180, D225, D270, D315

namespace dctl {
namespace board {

// primary template declaration
template<class, class>
struct ShiftSize;

// partial specialization definitions
template< class Grid > struct ShiftSize< Grid, angle::D000 >: Grid::right      {};
template< class Grid > struct ShiftSize< Grid, angle::D045 >: Grid::right_up   {};
template< class Grid > struct ShiftSize< Grid, angle::D090 >: Grid::up         {};
template< class Grid > struct ShiftSize< Grid, angle::D135 >: Grid::left_up    {};
template< class Grid > struct ShiftSize< Grid, angle::D180 >: Grid::left       {};
template< class Grid > struct ShiftSize< Grid, angle::D225 >: Grid::left_down  {};
template< class Grid > struct ShiftSize< Grid, angle::D270 >: Grid::down       {};
template< class Grid > struct ShiftSize< Grid, angle::D315 >: Grid::right_down {};

}       // namespace board
}       // namespace dctl
