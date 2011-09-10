#pragma once
#include <boost/config.hpp>

namespace dctl {
namespace board {

template
<
        typename Dimensions,    // dimensions
        int G = 0               // ghost columns
>
class Grid
: 
        public Dimensions
{
public:
        typedef Grid<Dimensions> BaseGrid;
        
        // diagonal directions
        BOOST_STATIC_CONSTANT(auto, left_down = (Dimensions::width + G) / 2);
        BOOST_STATIC_CONSTANT(auto, right_down = left_down + 1);

        // orthogonal directions
        BOOST_STATIC_CONSTANT(auto, right = right_down - left_down);     // == 1 by construction
        BOOST_STATIC_CONSTANT(auto, down = right_down + left_down);      // == 2 * ((width + G) / 2) + 1

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
        BOOST_STATIC_CONSTANT(auto, edge_lo = left_down + Dimensions::parity);
        BOOST_STATIC_CONSTANT(auto, edge_ro = edge_lo + BaseGrid::edge_ro - BaseGrid::edge_lo);

        // grid size 
        BOOST_STATIC_CONSTANT(auto, size = 
                modulo * ((Dimensions::height - 1) / 2) + 
                ((Dimensions::height % 2)? edge_re : edge_ro) + 1
        );
};

// partial specialization for grids without ghost columns
template<typename Dimensions>
class Grid<Dimensions, 0>
: 
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows
        BOOST_STATIC_CONSTANT(auto, row_e = (Dimensions::width +  Dimensions::parity) / 2);
        BOOST_STATIC_CONSTANT(auto, row_o = (Dimensions::width + !Dimensions::parity) / 2);

public:
        // range of row pairs
        BOOST_STATIC_CONSTANT(auto, modulo = Dimensions::width);

        // left (l) and right (r) edges of even (e) and odd (o) rows
        BOOST_STATIC_CONSTANT(auto, edge_le = 0);
        BOOST_STATIC_CONSTANT(auto, edge_re = edge_le + row_e - 1);
        BOOST_STATIC_CONSTANT(auto, edge_lo = edge_re + 1);
        BOOST_STATIC_CONSTANT(auto, edge_ro = edge_lo + row_o - 1);

        // == (H * W) / 2 + (H * W * P) % 2
        BOOST_STATIC_CONSTANT(auto, size = 
                modulo * ((Dimensions::height - 1) / 2) + 
                ((Dimensions::height % 2)? edge_re : edge_ro) + 1
        );
};

}       // namespace board
}       // namespace dctl
