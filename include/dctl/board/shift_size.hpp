#pragma once
#include <boost/mpl/int.hpp>
#include <dctl/angle/degrees.hpp>       // D00, D045, D090, D135, D180, D225, D270, D315

namespace dctl {
namespace board {

// primary template declaration
template<class, class>
struct ShiftSize;

// partial specialization definitions
template< class Grid > struct ShiftSize< Grid, angle::D000 >: boost::mpl::int_<Grid::right     > {};
template< class Grid > struct ShiftSize< Grid, angle::D045 >: boost::mpl::int_<Grid::right_up  > {};
template< class Grid > struct ShiftSize< Grid, angle::D090 >: boost::mpl::int_<Grid::up        > {};
template< class Grid > struct ShiftSize< Grid, angle::D135 >: boost::mpl::int_<Grid::left_up   > {};
template< class Grid > struct ShiftSize< Grid, angle::D180 >: boost::mpl::int_<Grid::left      > {};
template< class Grid > struct ShiftSize< Grid, angle::D225 >: boost::mpl::int_<Grid::left_down > {};
template< class Grid > struct ShiftSize< Grid, angle::D270 >: boost::mpl::int_<Grid::down      > {};
template< class Grid > struct ShiftSize< Grid, angle::D315 >: boost::mpl::int_<Grid::right_down> {};

}       // namespace board
}       // namespace dctl
