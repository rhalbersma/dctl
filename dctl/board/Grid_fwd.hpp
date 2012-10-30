#pragma once
#include <boost/mpl/int.hpp>            // int_

namespace dctl {
namespace board {

// primary template declaration
template<typename Dimensions, typename GhostColumns>
struct Grid;

typedef boost::mpl::int_<0> no_ghosts;

// partial template declaration
template<typename Dimensions>
struct Grid<Dimensions, no_ghosts>;

}       // namespace board
}       // namespace dctl
