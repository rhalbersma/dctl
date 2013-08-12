#pragma once
#include <dctl/angle/degrees.hpp>                       // D000, L090, R090, D180
#include <dctl/board/coordinates.hpp>                   // Coordinates
#include <dctl/board/coordinates/transform_fwd.hpp>     // rotate (partial specialization declarations)

namespace dctl {
namespace mpl {
namespace lazy {
namespace detail {

template<class T>
constexpr T dual(T const& size, T const& n)
{
        return size - n - 1;
}

}       // namespace detail

// NOTE: because rotate is a LAZY metafunction, 
// its primary template definition needs to have been seen at this point

// partial specialization definitions

// partial specialization for identity rotations
template<class Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::D000 >
:
        board::Coordinates<
                Grid,
                Row,
                Column
        >
{};

// partial specialization for 90 degrees left rotations
template<class Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::L090 >
:
        board::Coordinates<
                Grid,
                Column,
                detail::dual(Grid::height, Row)
        >
{};

// partial specialization for 90 degrees right rotations
template<class Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::R090 >
:
        board::Coordinates<
                Grid,
                detail::dual(Grid::width, Column),
                Row
        >
{};

// partial specialization for 180 degrees rotations
template<class Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::D180 >
:
        board::Coordinates<
                Grid,
                detail::dual(Grid::height, Row),
                detail::dual(Grid::width, Column)
        >
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
