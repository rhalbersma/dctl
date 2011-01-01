#include "JenkinsMix.h"
#include "../Position/Position.h"
#include "../Position/Reversible/Side.h"

template<typename Board, typename Index>
struct JenkinsHash<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                Index index_ = 0;

                JenkinsMix<Index>::mix(index_, p.to_move());
                JenkinsMix<Index>::mix(index_, p.pieces(Side::BLACK));
                JenkinsMix<Index>::mix(index_, p.pieces(Side::WHITE));
                JenkinsMix<Index>::mix(index_, p.kings());
                
                return index_;
        }
};
