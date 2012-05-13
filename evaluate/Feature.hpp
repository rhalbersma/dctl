#pragma once
#include <cstdlib>                      // abs
#include "Weight.hpp"
#include "../successor/Mobility.hpp"

namespace dctl {

template
<
        bool Color
>
class Feature
{
public:
        template<typename Position>
        static int evaluate(Position const& p)
        {
                int score = 0;
                score += material(p);
                score += tempo(p);
                score += center(p);
                score += balance(p);
                score += mobility(p);
                return score;
        }

        template<typename Rules, typename Board, template<typename, typename> class Position>
        static int material(Position<Rules, Board> const& p)
        {
                return (
                        Weight<Rules, Board>::material[0] * bit::count(p.pieces(Color)) +
                        Weight<Rules, Board>::material[1] * bit::count(p.kings(Color))
                );
        }

        template<typename Rules, typename Board, template<typename, typename> class Position>
        static int tempo(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::height; ++i)
                        score += Weight<Rules, Board>::tempo[i] * bit::count(p.pieces(Color) & Board::row_mask[Color][i]);
                return score;
        }

        template<typename Rules, typename Board, template<typename, typename> class Position>
        static int center(Position<Rules, Board> const& p)
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

        template<typename Rules, typename Board, template<typename, typename> class Position>
        static int balance(Position<Rules, Board> const& p)
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

        template<typename Rules, typename Board, template<typename, typename> class Position>
        static int mobility(Position<Rules, Board> const& p)
        {
                return Weight<Rules, Board>::mobility * Mobility<select::Moves>::template count<Color>(p);
        }
};

}       // namespace dctl
