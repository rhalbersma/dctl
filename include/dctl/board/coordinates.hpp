#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, minus, modulus, plus, times
#include <boost/mpl/bitwise.hpp>        // bitxor_
#include <boost/mpl/comparison.hpp>     // greater_equal
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // not_

namespace dctl {
namespace board {

template<class G, int R, int C>
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
        static constexpr auto P = C::row::value % 2;

        // number of row pairs
        static constexpr auto Q = C::row::value / 2;

        // the left edge
        static constexpr auto L = P? G::edge_lo : G::edge_le;

        // number of column pairs
        static constexpr auto S = C::col::value / 2;

        // squares from the left edge
        static constexpr auto R = (L + S) % G::modulo;

public:
        using type = Square<G, G::modulo *  Q + R>;
};

template<class SQ>
struct Square2Coordinates
{
private:
        using G = typename SQ::grid;

        // number of row pairs
        static constexpr auto Q =  SQ::value / G::modulo;

        // left edge of the zeroth row
        static constexpr auto R0 = SQ::value % G::modulo;

        // left edge of the first row
        static constexpr auto R1 = R0 - G::edge_lo;

        // R0 is in the zeroth or first row
        static constexpr auto P = R1 >= 0;

        // squares from the left edge
        static constexpr auto R = P? R1 : R0;

        // 2x the row pairs + the row parity
        static constexpr auto ROW = 2 * Q + P;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        static constexpr auto COL = 2 * R + (P ^ !G::parity);

public:
        using type = Coordinates<G, ROW, COL>;
};

}       // namespace board
}       // namespace dctl
