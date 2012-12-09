#pragma once
#include <boost/mpl/arithmetic.hpp>                     // minus
#include <boost/mpl/int.hpp>                            // int_
#include <dctl/angle/degrees.hpp>                       // D000, L090, R090, D180
#include <dctl/board/coordinates.hpp>                   // Coordinates
#include <dctl/board/coordinates/transform_fwd.hpp>     // rotate (partial specialization declarations)

namespace dctl {
namespace mpl {
namespace lazy {
namespace detail {

template<typename Size, int N>
struct Dual
:
        boost::mpl::minus<
                Size,
                boost::mpl::int_<1>,
                boost::mpl::int_<N>
        >
{};

}       // namespace detail

// NOTE: because rotate is a LAZY metafunction, 
// its primary template definition needs to have been seen at this point

// partial specialization definitions

// partial specialization for identity rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::D000 >
:
        board::Coordinates<
                Grid,
                Row,
                Column
        >
{};

// partial specialization for 90 degrees left rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::L090 >
:
        board::Coordinates<
                Grid,
                Column,
                detail::Dual< typename
                        Grid::height, 
                        Row
                >::value
        >
{};

// partial specialization for 90 degrees right rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::R090 >
:
        board::Coordinates<
                Grid,
                detail::Dual< typename
                        Grid::width, 
                        Column
                >::value,
                Row
        >
{};

// partial specialization for 180 degrees rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::D180 >
:
        board::Coordinates<
                Grid,
                detail::Dual< typename
                        Grid::height, 
                        Row
                >::value,
                detail::Dual< typename
                        Grid::width, 
                        Column
                >::value
        >
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
