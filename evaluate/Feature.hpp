#pragma once
#include <cstdlib>                      // abs
#include "Weight.hpp"
#include "../successor/Mobility.hpp"
#include "../node/Position.hpp"

namespace dctl { 
        
template<typename> struct Position;

template
<
        bool Color,
        typename Rules,
        typename Board
>
class Feature
{
public:
        static int evaluate(const Position<Board>& p)
        {
                int score = 0;
                score += material(p);/*
                score += tempo(p);
                score += center(p);
                score += balance(p);
                score += mobility(p);*/
                return score;
        }
        
        static int material(const Position<Board>& p)
        {
                return (
                        Weight<Rules, Board>::material[0] * bit::count(p.pieces(Color)) +
                        Weight<Rules, Board>::material[1] * bit::count(p.kings(Color))
                );
        }
        
        static int tempo(const Position<Board>& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::height; ++i)
                        score += Weight<Rules, Board>::tempo[i] * bit::count(p.pieces(Color) & Board::row_mask[Color][i]);
                return score;
        }
        
        static int center(const Position<Board>& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::width / 2; ++i)
                        score += Weight<Rules, Board>::center[i] *
                        (
                                bit::count(p.pieces(Color) & Board::col_mask[ Color][i]) +
                                bit::count(p.pieces(Color) & Board::col_mask[!Color][i])
                        );
                return score;
        }
        
        static int balance(const Position<Board>& p)
        {
                int score = 0;
                for (auto i = 0; i < Board::width / 2; ++i)
                        score += Weight<Rules, Board>::balance[i] *
                        (
                                bit::count(p.pieces(Color) & Board::col_mask[ Color][i]) -
                                bit::count(p.pieces(Color) & Board::col_mask[!Color][i])
                        );
                return -abs(score);
        }
        
        static int mobility(const Position<Board>& p)
        {
                return Weight<Rules, Board>::mobility * Mobility<select::Moves, Rules>::template count<Color>(p);
        }
};

}       // namespace dctl
