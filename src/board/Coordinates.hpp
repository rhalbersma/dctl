#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/arithmetic.hpp>     // minus, plus
#include <boost/mpl/int_fwd.hpp>        // int_
#include "Degrees.hpp"
#include "Modular.hpp"
#include "Transform.hpp"

namespace dctl {
namespace board {

template
<
        typename G, 
        int R, 
        int C
>
struct Coordinates
{
        typedef G grid;
        typedef boost::mpl::int_<R> row;
        typedef boost::mpl::int_<C> col;

        // lazily evaluable metadata == nullary metafunction
        typedef Coordinates<G, R, C> type;
};

template
<
        typename G, 
        typename N
>
struct Square
{
        typedef G grid;
        typedef N number;
};

template<typename C>
struct Coordinates2Square
{
private:
        typedef typename C::grid G;

        BOOST_STATIC_CONSTANT(auto, P = C::row::value % 2);             // row parity
        BOOST_STATIC_CONSTANT(auto, Q = C::row::value / 2);             // number of row pairs
        BOOST_STATIC_CONSTANT(auto, L = P? G::edge_lo : G::edge_le);    // the left edge
        BOOST_STATIC_CONSTANT(auto, S = C::col::value / 2);             // number of column pairs
        BOOST_STATIC_CONSTANT(auto, R = (L + S) % G::modulo);           // squares from the left edge

public:
        typedef Square<
                G,
                boost::mpl::int_<G::modulo * Q + R>
        > type;
};

template<typename SQ>
struct Square2Coordinates
{
private:
        typedef typename SQ::grid G;

        BOOST_STATIC_CONSTANT(auto, Q = SQ::number::value / G::modulo);         // number of row pairs
        BOOST_STATIC_CONSTANT(auto, R0 = SQ::number::value % G::modulo);        // left edge of the zeroth row
        BOOST_STATIC_CONSTANT(auto, R1 = R0 - G::edge_lo);                      // left edge of the first row
        BOOST_STATIC_CONSTANT(auto, P = R1 >= 0);                               // R0 is in the zeroth or first row
        BOOST_STATIC_CONSTANT(auto, R = P? R1 : R0);                            // squares from the left edge
        BOOST_STATIC_CONSTANT(auto, ROW = 2 * Q + P);
        BOOST_STATIC_CONSTANT(auto, COL = 2 * R + (P ^ !G::parity::value));

public:
        typedef Coordinates<
                G,
                ROW,    // 2x the row pairs + the row parity
                COL     // 2x the range from the left edge + the row parity XOR the opposite board coloring
        > type;
};

}       // namespace board

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
                boost::mpl::minus< typename 
                        Grid::height,
                        boost::mpl::int_<1>,
                        boost::mpl::int_<Row>
                >::value
        >
{}; 

// partial specialization for 90 degrees right rotations
template<typename Grid, int Row, int Column>
struct rotate< board::Coordinates<Grid, Row, Column>, angle::R090 >
:
        board::Coordinates<
                Grid, 
                boost::mpl::minus< typename 
                        Grid::width,
                        boost::mpl::int_<1>,
                        boost::mpl::int_<Column>
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
                boost::mpl::minus< typename
                        Grid::height,
                        boost::mpl::int_<1>,
                        boost::mpl::int_<Row>
                >::value,
                boost::mpl::minus< typename
                        Grid::width,
                        boost::mpl::int_<1>,
                        boost::mpl::int_<Column>
                >::value
        >
{};

}       // namespace dctl
