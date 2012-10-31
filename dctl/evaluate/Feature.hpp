#pragma once
#include <cstdlib>                      // abs
#include <dctl/evaluate/Weight.hpp>
#include <dctl/successor/Mobility.hpp>

namespace dctl {
namespace evaluate {

template
<
        bool Color
>
class Feature
{
public:
        template<typename Position>
        static int value(Position const& p)
        {
                int score = 0;
                score += material(p);
                score += tempo(p);
                score += center(p);
                score += balance(p);
                score += mobility(p);
                //score += king_monopoly(p);
                return (score);
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int material(Position<Rules, Board> const& p)
        {
                return (
                        Weight<Rules, Board>::material[0] * bit::count(p.pieces(Color)) +
                        Weight<Rules, Board>::material[1] * bit::count(p.kings(Color))
                );
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int tempo(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::height::value; ++i) {
                        score += Weight<Rules, Board>::tempo[i] * bit::count(p.pieces(Color) & Board::row_mask[Color][i]);
                }
                return (score);
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int center(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 1; i < Board::width::value / 2; ++i) {
                        score += Weight<Rules, Board>::center[i] *
                        (
                                bit::count(p.pieces(Color) & Board::col_mask[ Color][i]) +
                                bit::count(p.pieces(Color) & Board::col_mask[!Color][i])
                        );
                }
                return (score);
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int balance(Position<Rules, Board> const& p)
        {
                int score = 0;
                for (auto i = 0; i < Board::width::value / 2; ++i) {
                        score += Weight<Rules, Board>::balance[i] *
                        (
                                bit::count(p.pieces(Color) & Board::col_mask[ Color][i]) -
                                bit::count(p.pieces(Color) & Board::col_mask[!Color][i])
                        );
                }
                return (-abs(score));
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int mobility(Position<Rules, Board> const& p)
        {
                return (Weight<Rules, Board>::mobility * successor::mobility<Color>(p));
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int king_monopoly(Position<Rules, Board> const& p)
        {
                return (Weight<Rules, Board>::king_monopoly * (!bit::is_zero(p.kings(Color)) && bit::is_zero(p.kings(!Color))));
        }
};

}       // namespace evaluate
}       // namespace dctl
