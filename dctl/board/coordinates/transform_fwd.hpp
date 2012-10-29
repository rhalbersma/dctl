#pragma once
#include <dctl/angle/Degrees.hpp>       // D000, L090, R090, D180
#include <dctl/board/Coordinates.hpp>   // Coordinates
#include <dctl/mpl/transform.hpp>       // rotate (primary template definition)

namespace dctl {
namespace mpl {
namespace lazy {

// partial specialization declarations

// partial specialization for identity rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::D000 >;

// partial specialization for 90 degrees left rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::L090 >;

// partial specialization for 90 degrees right rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::R090 >;

// partial specialization for 180 degrees rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::D180 >;

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
