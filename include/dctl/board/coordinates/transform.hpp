#pragma once
#include <dctl/angle/degrees.hpp>                       // D000, L090, R090, D180
#include <dctl/board/coordinates.hpp>                   // Coordinates
#include <dctl/board/coordinates/transform_fwd.hpp>     // rotate (partial specialization declarations)

namespace dctl {
namespace mpl {
namespace lazy {

// NOTE: because rotate is a LAZY metafunction, 
// its primary template definition needs to have been seen at this point

// partial specialization definitions

// partial specialization for identity rotations
template<int Row, int Column>
struct rotate< board::Coordinates<Row, Column>, angle::D000 >
:
        board::Coordinates<Row, Column>
{};

// partial specialization for 90 degrees left rotations
template<int Row, int Column>
struct rotate< board::Coordinates<Row, Column>, angle::L090 >
:
        board::Coordinates<Column, -Row>
{};

// partial specialization for 90 degrees right rotations
template<int Row, int Column>
struct rotate< board::Coordinates<Row, Column>, angle::R090 >
:
        board::Coordinates<-Column, Row>
{};

// partial specialization for 180 degrees rotations
template<int Row, int Column>
struct rotate< board::Coordinates<Row, Column>, angle::D180 >
:
        board::Coordinates<-Row, -Column>
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
