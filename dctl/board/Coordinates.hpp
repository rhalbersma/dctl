#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, minus, modulus, plus, times
#include <boost/mpl/bitwise.hpp>        // bitxor_
#include <boost/mpl/comparison.hpp>     // greater_equal
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // not_

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

        // row parity
        typedef boost::mpl::modulus< typename
                C::row,
                boost::mpl::int_<2>
        > P;

        // number of row pairs
        typedef boost::mpl::divides< typename
                C::row,
                boost::mpl::int_<2>
        > Q;

        // the left edge
        typedef boost::mpl::eval_if<
                P, typename
                G::edge_lo, typename
                G::edge_le
        > L;

        // number of column pairs
        typedef boost::mpl::divides< typename
                C::col,
                boost::mpl::int_<2>
        > S;

        // squares from the left edge
        typedef boost::mpl::modulus<
                boost::mpl::plus<
                        L,
                        S
                >, typename
                G::modulo
        > R;

public:
        typedef Square<
                G, typename
                boost::mpl::plus<
                        boost::mpl::times< typename
                                G::modulo,
                                Q
                        >,
                        R
                >::type
        > type;
};

template<typename SQ>
struct Square2Coordinates
{
private:
        typedef typename SQ::grid G;

        // number of row pairs
        typedef boost::mpl::divides< typename
                SQ::number, typename
                G::modulo
        > Q;

        // left edge of the zeroth row
        typedef boost::mpl::modulus< typename
                SQ::number, typename
                G::modulo
        > R0;

        // left edge of the first row
        typedef boost::mpl::minus<
                R0, typename
                G::edge_lo
        > R1;

        // R0 is in the zeroth or first row
        typedef boost::mpl::greater_equal<
                R1,
                boost::mpl::int_<0>
        > P;

        // squares from the left edge
        typedef boost::mpl::eval_if< P, R1, R0 > R;

        // 2x the row pairs + the row parity
        typedef boost::mpl::plus<
                boost::mpl::times<
                        boost::mpl::int_<2>,
                        Q
                >,
                P
        > ROW;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        typedef boost::mpl::plus<
                boost::mpl::times<
                        boost::mpl::int_<2>,
                        R
                >,
                boost::mpl::bitxor_<
                        P,
                        boost::mpl::not_< typename
                                G::parity
                        >
                >
        > COL;

public:
        typedef Coordinates<
                G,
                ROW::value,
                COL::value
        > type;
};

}       // namespace board
}       // namespace dctl
