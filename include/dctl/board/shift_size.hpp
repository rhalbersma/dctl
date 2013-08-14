#pragma once
#include <boost/mpl/int.hpp>
#include <dctl/angle/degrees.hpp>       // D00, D045, D090, D135, D180, D225, D270, D315

namespace dctl {
namespace board {

// primary template declaration
template<class, int>
struct ShiftSize;

// partial specialization definitions
template< class Grid > struct ShiftSize< Grid, angle::D000::value >: boost::mpl::int_<Grid::right     > {};
template< class Grid > struct ShiftSize< Grid, angle::D045::value >: boost::mpl::int_<Grid::right_up  > {};
template< class Grid > struct ShiftSize< Grid, angle::D090::value >: boost::mpl::int_<Grid::up        > {};
template< class Grid > struct ShiftSize< Grid, angle::D135::value >: boost::mpl::int_<Grid::left_up   > {};
template< class Grid > struct ShiftSize< Grid, angle::D180::value >: boost::mpl::int_<Grid::left      > {};
template< class Grid > struct ShiftSize< Grid, angle::D225::value >: boost::mpl::int_<Grid::left_down > {};
template< class Grid > struct ShiftSize< Grid, angle::D270::value >: boost::mpl::int_<Grid::down      > {};
template< class Grid > struct ShiftSize< Grid, angle::D315::value >: boost::mpl::int_<Grid::right_down> {};

}       // namespace board
}       // namespace dctl
