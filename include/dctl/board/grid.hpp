#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, minus, modulus, plus, times
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/logical.hpp>        // not_
#include <boost/mpl/int.hpp>            // int_
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include <dctl/board/grid_fwd.hpp>      // primary template and partial specialization declarations

namespace dctl {
namespace board {

// primary template definition
template
<
        class Dimensions,
        class GhostColumns
>
struct Grid
:
        public Dimensions
{
public:
        BOOST_STATIC_ASSERT(GhostColumns::value > 0);

        using BaseGrid = Grid<Dimensions, no_ghosts>;

        // diagonal directions

        using left_down = boost::mpl::divides<
                boost::mpl::plus< typename
                        Dimensions::width,
                        GhostColumns
                >,
                boost::mpl::int_<2>
        >;

        using right_down = boost::mpl::plus<
                left_down,
                boost::mpl::int_<1>
        >;

        // orthogonal directions

        using right = boost::mpl::minus<
                right_down,
                left_down
        >;

        using down = boost::mpl::plus<
                right_down,
                left_down
        >;

        // equivalent directions

        using left_up = right_down;
        using right_up = left_down;
        using left = right;
        using up = down;

        // range of row pairs

        using modulo = down;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        using edge_le = typename BaseGrid::edge_le;
        using edge_re = typename BaseGrid::edge_re;

        using edge_lo = boost::mpl::plus<
                left_down, typename
                Dimensions::parity
        >;

        using edge_ro = boost::mpl::plus<
                edge_lo,
                boost::mpl::minus< typename
                        BaseGrid::edge_ro, typename
                        BaseGrid::edge_lo
                >
        >;

        // grid size

        using size = boost::mpl::plus<
                boost::mpl::times<
                        modulo,
                        boost::mpl::divides<
                                boost::mpl::minus< typename
                                        Dimensions::height,
                                        boost::mpl::int_<1>
                                >,
                                boost::mpl::int_<2>
                        >
                >, typename
                boost::mpl::eval_if<
                        boost::mpl::modulus< typename
                                Dimensions::height,
                                boost::mpl::int_<2>
                        >,
                        edge_re,
                        edge_ro
                >::type,
                boost::mpl::int_<1>
        >;
};

// partial specialization definition
template<class Dimensions>
struct Grid< Dimensions, no_ghosts >
:
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows

        using row_e = boost::mpl::divides<
                boost::mpl::plus< typename
                        Dimensions::width, typename
                        Dimensions::parity
                >,
                boost::mpl::int_<2>
        >;

        using row_o = boost::mpl::divides<
                boost::mpl::plus< typename
                        Dimensions::width,
                        boost::mpl::not_< typename
                                Dimensions::parity
                        >
                >,
                boost::mpl::int_<2>
        >;

public:
        // range of row pairs

        using modulo = typename Dimensions::width;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        using edge_le = boost::mpl::int_<0>;

        using edge_re = boost::mpl::plus<
                edge_le,
                boost::mpl::minus<
                        row_e,
                        boost::mpl::int_<1>
                >
        >;

        using edge_lo = boost::mpl::plus<
                edge_re,
                boost::mpl::int_<1>
        >;

        using edge_ro = boost::mpl::plus<
                edge_lo,
                boost::mpl::minus<
                        row_o,
                        boost::mpl::int_<1>
                >
        >;

        using size = boost::mpl::plus<
                boost::mpl::times<
                        modulo,
                        boost::mpl::divides<
                                boost::mpl::minus< typename
                                        Dimensions::height,
                                        boost::mpl::int_<1>
                                >,
                                boost::mpl::int_<2>
                        >
                >, typename
                boost::mpl::eval_if<
                        boost::mpl::modulus< typename
                                Dimensions::height,
                                boost::mpl::int_<2>
                        >,
                        edge_re,
                        edge_ro
                >::type,
                boost::mpl::int_<1>
        >;

        // equivalent grid size
        /*
        BOOST_STATIC_ASSERT(size == (Dimensions::height::value * Dimensions::width::value) / 2 +
                (Dimensions::parity::value * Dimensions::height::value * Dimensions::width::value) % 2
        );
        */
};

}       // namespace board
}       // namespace dctl
