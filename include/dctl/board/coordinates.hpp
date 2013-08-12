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
        class G,
        int R,
        int C
>
struct Coordinates
{
        using grid = G;
        using row = boost::mpl::int_<R>;
        using col = boost::mpl::int_<C>;

        // lazily evaluable metadata == nullary metafunction
        using type = Coordinates<G, R, C>;
};

template<class G, int N>
struct Square
{
        using grid = G;
        static constexpr auto value = N;
};

template<class C>
struct Coordinates2Square
{
private:
        using G = typename C::grid;

        // row parity
        using P = boost::mpl::modulus< typename
                C::row,
                boost::mpl::int_<2>
        >;

        // number of row pairs
        using Q = boost::mpl::divides< typename
                C::row,
                boost::mpl::int_<2>
        >;

        // the left edge
        using L = typename boost::mpl::eval_if<
                P,
                boost::mpl::int_<G::edge_lo>,
                boost::mpl::int_<G::edge_le>
        >::type;

        // number of column pairs
        using S = boost::mpl::divides< typename
                C::col,
                boost::mpl::int_<2>
        >;

        // squares from the left edge
        using R = boost::mpl::modulus<
                boost::mpl::plus<
                        L,
                        S
                >,
                boost::mpl::int_<G::modulo>
        >;

public:
        using type = Square<
                G,
                boost::mpl::plus<
                        boost::mpl::times<
                                boost::mpl::int_<G::modulo>,
                                Q
                        >,
                        R
                >::value
        >;
};

template<class SQ>
struct Square2Coordinates
{
private:
        using G = typename SQ::grid;

        // number of row pairs
        using Q = boost::mpl::divides< typename
                boost::mpl::int_<SQ::value>,
                boost::mpl::int_<G::modulo>
        >;

        // left edge of the zeroth row
        using R0 = boost::mpl::modulus< typename
                boost::mpl::int_<SQ::value>,
                boost::mpl::int_<G::modulo>
        >;

        // left edge of the first row
        using R1 = boost::mpl::minus<
                R0,
                boost::mpl::int_<G::edge_lo>
        >;

        // R0 is in the zeroth or first row
        using P = boost::mpl::greater_equal<
                R1,
                boost::mpl::int_<0>
        >;

        // squares from the left edge
        using R = typename boost::mpl::eval_if< P, R1, R0 >::type;

        // 2x the row pairs + the row parity
        using ROW = boost::mpl::plus<
                boost::mpl::times<
                        boost::mpl::int_<2>,
                        Q
                >,
                P
        >;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        using COL = boost::mpl::plus<
                boost::mpl::times<
                        boost::mpl::int_<2>,
                        R
                >,
                boost::mpl::bitxor_<
                        P,
                        boost::mpl::not_<
                                boost::mpl::bool_<G::parity>
                        >
                >
        >;

public:
        using type = Coordinates<
                G,
                ROW::value,
                COL::value
        >;
};

}       // namespace board
}       // namespace dctl
