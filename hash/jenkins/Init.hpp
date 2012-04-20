#pragma once
#include <functional>                   // unary_function
#include "Random.hpp"
#include "../../node/Side.hpp"

namespace dctl {
namespace hash {
namespace jenkins {

// primary template
template<typename Key, typename Index>
struct Init;

// partial specialization for ab initio hashing of positions
template<typename Rules, typename Board, template<typename, typename> class Position, typename Index>
struct Init<Position<Rules, Board>, Index>
:
        public std::unary_function<Position<Rules, Board>, Index>
{
        Index operator()(const Position<Rules, Board>& p) const
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
