#pragma once
#include <boost/mpl/int.hpp>            // int_

namespace dctl {
namespace board {

// primary template declaration
template<class Dimensions, class GhostColumns>
struct Grid;

using no_ghosts = boost::mpl::int_<0>;

// partial template declaration
template<class Dimensions>
struct Grid<Dimensions, no_ghosts>;

}       // namespace board
}       // namespace dctl
