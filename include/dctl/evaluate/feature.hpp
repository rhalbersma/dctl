#pragma once
#include <dctl/color.hpp>               // Color
#include <dctl/piece.hpp>
#include <dctl/board/mask.hpp>          // Column, Row
#include <dctl/evaluate/weight.hpp>     // Weight
#include <dctl/successor.hpp>           // count, select::push
#include <cstdlib>                      // abs

namespace dctl {
namespace evaluate {

template<Color ToMove>
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
                        Weight<Rules, Board>::material[0] * static_cast<int>(p.num_pieces(ToMove, Piece::pawn)) +
                        Weight<Rules, Board>::material[1] * static_cast<int>(p.num_pieces(ToMove, Piece::king))
                ;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int tempo(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::height(); ++i)
                        score += Weight<Rules, Board>::tempo[i] * static_cast<int>((p.pieces(ToMove) & board::Row<Board>::mask(ToMove, i)).count());
                return score;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int center(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::width() / 2; ++i) {
                        score += Weight<Rules, Board>::center[i] *
                        (
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask( ToMove, i)).count()) +
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask(!ToMove, i)).count())
                        );
                }
                return score;
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int balance(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 0; i < Board::width() / 2; ++i) {
                        score += Weight<Rules, Board>::balance[i] *
                        (
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask( ToMove, i)).count()) -
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask(!ToMove, i)).count())
                        );
                }
                return -abs(score);
        }

        template<template<class, class> class Position, class Rules, class Board>
        static int mobility(Position<Rules, Board> const& p)
        {
                return Weight<Rules, Board>::mobility * static_cast<int>(successor::Successor<successor::select::push>{}.template count<ToMove>(p));
        }
};

}       // namespace evaluate
}       // namespace dctl
