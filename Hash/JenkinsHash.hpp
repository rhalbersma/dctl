#include "JenkinsMix.h"
#include "../Position/Position.h"
#include "../Position/Reversible/Side.h"

template<typename Board, typename Index>
struct JenkinsHash<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                Index d_index = 0;

                JenkinsMix<Index>::mix(d_index, p.to_move());
                JenkinsMix<Index>::mix(d_index, p.pieces(Side::BLACK));
                JenkinsMix<Index>::mix(d_index, p.pieces(Side::WHITE));
                JenkinsMix<Index>::mix(d_index, p.kings());
                
                return d_index;
        }
};
