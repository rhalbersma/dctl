#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/int_fwd.hpp>        // int_
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
        BOOST_STATIC_CONSTANT(auto, left_down = (Dimensions::width::value + Ghosts::value) / 2);
        BOOST_STATIC_CONSTANT(auto, right_down = left_down + 1);

        // orthogonal directions
        BOOST_STATIC_CONSTANT(auto, right = right_down - left_down);
        BOOST_STATIC_CONSTANT(auto, down = right_down + left_down);

        // equivalent directions
        BOOST_STATIC_CONSTANT(auto, left_up = right_down);
        BOOST_STATIC_CONSTANT(auto, right_up = left_down);
        BOOST_STATIC_CONSTANT(auto, left = right);
        BOOST_STATIC_CONSTANT(auto, up = down);

        // range of row pairs
        BOOST_STATIC_CONSTANT(auto, modulo = down);

        // left (l) and right (r) edges of even (e) and odd (o) rows
        BOOST_STATIC_CONSTANT(auto, edge_le = BaseGrid::edge_le);
        BOOST_STATIC_CONSTANT(auto, edge_re = BaseGrid::edge_re);
        BOOST_STATIC_CONSTANT(auto, edge_lo = left_down + Dimensions::parity::value);
        BOOST_STATIC_CONSTANT(auto, edge_ro = edge_lo + BaseGrid::edge_ro - BaseGrid::edge_lo);

        // grid size
        BOOST_STATIC_CONSTANT(auto, size =
                modulo * ((Dimensions::height::value - 1) / 2) +
                ((Dimensions::height::value % 2)? edge_re : edge_ro) + 1
        );

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(push)
#pragma warning(disable:4348)
#endif
/*---------------------------------------------------------------------------*/

        // Warning C4348 is incorrectly generated for nested templates with default parameters
        // http://connect.microsoft.com/VisualStudio/feedback/details/474655/warning-c4348-is-incorrectly-generated-for-nested-templates
        template<typename Index, class _ = void> struct shift;

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(pop)
#endif
/*---------------------------------------------------------------------------*/

        template< class _ > struct shift< angle::D000, _ >: boost::mpl::int_< right      > {};
        template< class _ > struct shift< angle::D045, _ >: boost::mpl::int_< right_up   > {};
        template< class _ > struct shift< angle::D090, _ >: boost::mpl::int_< up         > {};
        template< class _ > struct shift< angle::D135, _ >: boost::mpl::int_< left_up    > {};
        template< class _ > struct shift< angle::D180, _ >: boost::mpl::int_< left       > {};
        template< class _ > struct shift< angle::D225, _ >: boost::mpl::int_< left_down  > {};
        template< class _ > struct shift< angle::D270, _ >: boost::mpl::int_< down       > {};
        template< class _ > struct shift< angle::D315, _ >: boost::mpl::int_< right_down > {};
};

// partial specialization for grids without ghost columns
template<typename Dimensions>
class Grid< Dimensions, boost::mpl::int_<0> >
:
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows
        BOOST_STATIC_CONSTANT(auto, row_e = (Dimensions::width::value +  Dimensions::parity::value) / 2);
        BOOST_STATIC_CONSTANT(auto, row_o = (Dimensions::width::value + !Dimensions::parity::value) / 2);

public:
        // range of row pairs
        BOOST_STATIC_CONSTANT(auto, modulo = Dimensions::width::value);

        // left (l) and right (r) edges of even (e) and odd (o) rows
        BOOST_STATIC_CONSTANT(auto, edge_le = 0);
        BOOST_STATIC_CONSTANT(auto, edge_re = edge_le + row_e - 1);
        BOOST_STATIC_CONSTANT(auto, edge_lo = edge_re + 1);
        BOOST_STATIC_CONSTANT(auto, edge_ro = edge_lo + row_o - 1);

        // grid size
        BOOST_STATIC_CONSTANT(auto, size =
                modulo * ((Dimensions::height::value - 1) / 2) +
                ((Dimensions::height::value % 2)? edge_re : edge_ro) + 1
        );

        // equivalent grid size
        BOOST_STATIC_ASSERT(size == (Dimensions::height::value * Dimensions::width::value) / 2 +
                (Dimensions::parity::value * Dimensions::height::value * Dimensions::width::value) % 2
        );
};

}       // namespace board
}       // namespace dctl