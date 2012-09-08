#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, minus, modulus, plus, times
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/logical.hpp>        // not_
#include <boost/mpl/int.hpp>            // int_
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Degrees.hpp"

namespace dctl {
namespace board {

template
<
        typename Dimensions,                    // dimensions
        typename Ghosts = boost::mpl::int_<0>   // ghost columns
>
struct Grid
:
        Dimensions
{
public:
        BOOST_STATIC_ASSERT(Ghosts::value > 0);

        typedef Grid<Dimensions> BaseGrid;

        // diagonal directions

        typedef boost::mpl::divides<
                boost::mpl::plus< typename
                        Dimensions::width,
                        Ghosts
                >, 
                boost::mpl::int_<2>                
        > left_down;

        typedef boost::mpl::plus<
                left_down, 
                boost::mpl::int_<1>
        > right_down;

        // orthogonal directions

        typedef boost::mpl::minus<
                right_down, 
                left_down
        > right;

        typedef boost::mpl::plus<
                right_down, 
                left_down
        > down;

        // equivalent directions

        typedef right_down      left_up;
        typedef left_down       right_up;
        typedef right           left;
        typedef down            up;

        // range of row pairs

        typedef down modulo;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        typedef typename BaseGrid::edge_le edge_le;
        typedef typename BaseGrid::edge_re edge_re;
        
        typedef boost::mpl::plus<
                left_down, typename
                Dimensions::parity
        > edge_lo;

        typedef boost::mpl::plus<
                edge_lo,
                boost::mpl::minus< typename
                        BaseGrid::edge_ro, typename
                        BaseGrid::edge_lo
                >
        > edge_ro;

        // grid size

        typedef boost::mpl::plus<
                boost::mpl::times<
                        modulo,
                        boost::mpl::divides<
                                boost::mpl::minus< typename
                                        Dimensions::height,
                                        boost::mpl::int_<1>
                                >,
                                boost::mpl::int_<2>
                        >
                >,
                boost::mpl::eval_if<
                        boost::mpl::modulus< typename
                                Dimensions::height,
                                boost::mpl::int_<2>
                        >,
                        edge_re,
                        edge_ro
                >,
                boost::mpl::int_<1>
        > size;

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(push)
#pragma warning(disable:4348)
#endif
/*---------------------------------------------------------------------------*/

        // Warning C4348 is incorrectly generated for nested templates with default parameters
        // http://connect.microsoft.com/VisualStudio/feedback/details/474655/warning-c4348-is-incorrectly-generated-for-nested-templates
        template<typename Index, typename dummy = void> struct shift;

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(pop)
#endif
/*---------------------------------------------------------------------------*/

        template< typename dummy > struct shift< angle::D000, dummy >: right      {};
        template< typename dummy > struct shift< angle::D045, dummy >: right_up   {};
        template< typename dummy > struct shift< angle::D090, dummy >: up         {};
        template< typename dummy > struct shift< angle::D135, dummy >: left_up    {};
        template< typename dummy > struct shift< angle::D180, dummy >: left       {};
        template< typename dummy > struct shift< angle::D225, dummy >: left_down  {};
        template< typename dummy > struct shift< angle::D270, dummy >: down       {};
        template< typename dummy > struct shift< angle::D315, dummy >: right_down {};
};

// partial specialization for grids without ghost columns
template<typename Dimensions>
class Grid< Dimensions, boost::mpl::int_<0> >
:
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows

        typedef boost::mpl::divides<
                boost::mpl::plus< typename
                        Dimensions::width, typename
                        Dimensions::parity
                >,
                boost::mpl::int_<2>
        > row_e;

        typedef boost::mpl::divides<
                boost::mpl::plus< typename
                        Dimensions::width,
                        boost::mpl::not_< typename
                                Dimensions::parity
                        >
                >,
                boost::mpl::int_<2>
        > row_o;

public:
        // range of row pairs
        
        typedef typename Dimensions::width modulo;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        typedef boost::mpl::int_<0> edge_le;
        
        typedef boost::mpl::plus<
                edge_le,
                boost::mpl::minus<
                        row_e,
                        boost::mpl::int_<1>
                >
        > edge_re;

        typedef boost::mpl::plus<
                edge_re,
                boost::mpl::int_<1>
        > edge_lo;

        typedef boost::mpl::plus<
                edge_lo,
                boost::mpl::minus<
                        row_o,
                        boost::mpl::int_<1>
                >
        > edge_ro;

        typedef boost::mpl::plus<
                boost::mpl::times<
                        modulo,
                        boost::mpl::divides<
                                boost::mpl::minus< typename
                                        Dimensions::height,
                                        boost::mpl::int_<1>
                                >,
                                boost::mpl::int_<2>
                        >
                >,
                boost::mpl::eval_if<
                        boost::mpl::modulus< typename
                                Dimensions::height,
                                boost::mpl::int_<2>
                        >,
                        edge_re,
                        edge_ro
                >,
                boost::mpl::int_<1>
        > size;

        // equivalent grid size
        /*
        BOOST_STATIC_ASSERT(size == (Dimensions::height::value * Dimensions::width::value) / 2 +
                (Dimensions::parity::value * Dimensions::height::value * Dimensions::width::value) % 2
        );
        */
};

}       // namespace board
}       // namespace dctl
