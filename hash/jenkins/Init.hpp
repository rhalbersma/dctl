#include "Mix.h"
#include "../../node/Position.h"
#include "../../node/Side.hpp"

namespace dctl {
namespace hash {
namespace jenkins {

template<typename Board, typename Index>
struct Init<Position<Board>, Index>
: 
        public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                Index index_ = 0;

                Mix<Index>::mix(index_, p.active_color());
                Mix<Index>::mix(index_, p.pieces(Side::black));
                Mix<Index>::mix(index_, p.pieces(Side::white));
                Mix<Index>::mix(index_, p.kings());
                
                return index_;
        }
};

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl
