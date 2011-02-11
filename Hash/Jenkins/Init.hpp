#include "Mix.h"
#include "../Position/Position.h"
#include "../Position/Side.h"

namespace Hash {
namespace Jenkins {

template<typename Board, typename Index>
struct Init<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                Index index_ = 0;

                Mix<Index>::mix(index_, p.to_move());
                Mix<Index>::mix(index_, p.pieces(Side::BLACK));
                Mix<Index>::mix(index_, p.pieces(Side::WHITE));
                Mix<Index>::mix(index_, p.kings());
                
                return index_;
        }
};

}       // namespace Jenkins
}       // namespace Hash
