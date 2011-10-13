#pragma once
#include "Feature.hpp"

namespace dctl { 
        
template<typename> struct Position;

template
<
        bool Color,
        typename Rules,
        typename Board
>
class Delta
{
public:
        static int evaluate(const Position<Board>& p)
        {
                return Feature<Color, Rules, Board>::evaluate(p) - Feature<!Color, Rules, Board>::evaluate(p);
        }
        
private:
        static int material(const Position<Board>& p)
        {
                return Feature<Color, Rules, Board>::material(p) - Feature<!Color, Rules, Board>::material(p);
        }
        
        static int tempo(const Position<Board>& p)
        {
                return Feature<Color, Rules, Board>::tempo(p) - Feature<!Color, Rules, Board>::tempo(p);
        }
        
        static int center(const Position<Board>& p)
        {
                return Feature<Color, Rules, Board>::center(p) - Feature<!Color, Rules, Board>::center(p);
        }
        
        static int balance(const Position<Board>& p)
        {
                return Feature<Color, Rules, Board>::balance(p) - Feature<!Color, Rules, Board>::balance(p);
        }
        
        static int mobility(const Position<Board>& p)
        {
                return Feature<Color, Rules, Board>::mobility(p) - Feature<!Color, Rules, Board>::mobility(p);
        }
};

}       // namespace dctl
