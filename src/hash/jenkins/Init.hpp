#pragma once
#include <functional>                   // function
#include "Random.hpp"
#include "../../node/Side.hpp"

namespace dctl {
namespace hash {
namespace jenkins {

// primary template
template<typename Index, typename Key>
struct Init;

// partial specialization for ab initio hashing of positions
template<typename Index, typename Rules, typename Board, template<typename, typename> class Position>
struct Init< Index, Position<Rules, Board> >
:
        std::function<Index(Position<Rules, Board>)>
{
        Index operator()(Position<Rules, Board> const& p) const
        {
                Index index = 0;

                Random<Index>::mix(index, p.active_color());
                Random<Index>::mix(index, p.pieces(Side::black));
                Random<Index>::mix(index, p.pieces(Side::white));
                Random<Index>::mix(index, p.kings());

                return index;
        }
};

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl
