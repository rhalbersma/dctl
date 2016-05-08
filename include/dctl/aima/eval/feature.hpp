#pragma once
#include <dctl/aima/eval/weight.hpp>      // Weight
#include <dctl/color.hpp>               // Player
#include <dctl/piece.hpp>
#include <dctl/board/mask.hpp>          // Column, Row
#include <dctl/actions.hpp>             // count, select::push
#include <xstd/cstddef.hpp>
#include <cstdlib>                      // abs

namespace dctl {
namespace evaluate {

template<Color ToMove>
class Feature
{
public:
        template<class State>
        static int value(State const& p)
        {
                int score = 0;
                score += material(p);
                score += tempo(p);
                score += center(p);
                score += balance(p);
                score += mobility(p);
                return score;
        }

        template<template<class, class> class State, class Rules, class Board>
        static int material(State<Rules, Board> const& p)
        {
                return
                        Weight<Rules, Board>::material[0] * static_cast<int>(p.num_pieces(ToMove, Piece::pawn)) +
                        Weight<Rules, Board>::material[1] * static_cast<int>(p.num_pieces(ToMove, Piece::king))
                ;
        }

        template<template<class, class> class State, class Rules, class Board>
        static int tempo(State<Rules, Board> const& p)
        {
                using namespace xstd::support_literals;
                int score = 0;
                for (auto i = 1_zu; i < Board::height(); ++i)
                        score += Weight<Rules, Board>::tempo[i] * static_cast<int>((p.pieces(ToMove) & board::Row<Board>::mask(ToMove, i)).count());
                return score;
        }

        template<template<class, class> class State, class Rules, class Board>
        static int center(State<Rules, Board> const& p)
        {
                using namespace xstd::support_literals;
                int score = 0;
                for (auto i = 1_zu; i < Board::width() / 2; ++i) {
                        score += Weight<Rules, Board>::center[i] *
                        (
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask( ToMove, i)).count()) +
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask(!ToMove, i)).count())
                        );
                }
                return score;
        }

        template<template<class, class> class State, class Rules, class Board>
        static int balance(State<Rules, Board> const& p)
        {
                using namespace xstd::support_literals;
                int score = 0;
                for (auto i = 0_zu; i < Board::width() / 2; ++i) {
                        score += Weight<Rules, Board>::balance[i] *
                        (
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask( ToMove, i)).count()) -
                                static_cast<int>((p.pieces(ToMove) & board::Column<Board>::mask(!ToMove, i)).count())
                        );
                }
                return -abs(score);
        }

        template<template<class, class> class State, class Rules, class Board>
        static int mobility(State<Rules, Board> const& p)
        {
                return Weight<Rules, Board>::mobility * static_cast<int>(core::Actions<core::select::push>{}.template count<ToMove>(p));
        }
};

}       // namespace evaluate
}       // namespace dctl
