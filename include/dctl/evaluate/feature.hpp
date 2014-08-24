#pragma once
#include <cstdlib>                      // abs
#include <dctl/evaluate/weight.hpp>
#include <dctl/successor/mobility.hpp>
#include <dctl/board/mask.hpp>

namespace dctl {
namespace evaluate {

template<bool Color>
class Feature
{
public:
        template<class Position>
        static int value(Position const& p)
        {
                int score = 0;
                score += material(p);
                score += tempo(p);
                score += center(p);
                score += balance(p);
                score += mobility(p);
                return score;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int material(Position<Rules, Board> const& p)
        {
                return
                        Weight<Rules, Board>::material[0] * p.pieces(Color).count() +
                        Weight<Rules, Board>::material[1] * p.kings(Color).count()
                ;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int tempo(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::height; ++i)
                        score += Weight<Rules, Board>::tempo[i] * (p.pieces(Color) & board::Row<Board>::mask(Color, i)).count();
                return score;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int center(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::width / 2; ++i) {
                        score += Weight<Rules, Board>::center[i] *
                        (
                                (p.pieces(Color) & board::Column<Board>::mask( Color, i)).count() +
                                (p.pieces(Color) & board::Column<Board>::mask(!Color, i)).count()
                        );
                }
                return score;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int balance(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 0; i < Board::width / 2; ++i) {
                        score += Weight<Rules, Board>::balance[i] *
                        (
                                (p.pieces(Color) & board::Column<Board>::mask( Color, i)).count() -
                                (p.pieces(Color) & board::Column<Board>::mask(!Color, i)).count()
                        );
                }
                return -abs(score);
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int mobility(Position<Rules, Board> const& p)
        {
                return Weight<Rules, Board>::mobility * successor::mobility<Color>(p);
        }
};

}       // namespace evaluate
}       // namespace dctl
