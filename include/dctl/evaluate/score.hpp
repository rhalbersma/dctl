#pragma once
#include <iomanip>
#include <iostream>
#include <dctl/evaluate/delta.hpp>
#include <dctl/evaluate/feature.hpp>
#include <dctl/evaluate/weight.hpp>

namespace dctl {
namespace evaluate {

template<class Position>
static int score (Position const& p)
{
        return (p.to_move() == Color::black) ?
                Delta<Color::black>::value(p) :
                Delta<Color::white>::value(p)
        ;
}

template<class Position>
static void print_break_down(Position const& p)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Move    " << std::setw(6) << Feature<Color::black>::material(p) << std::setw(6) << Feature<Color::white>::material(p) << std::setw(6) << Delta<Color::black>::material(p) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<Color::black>::tempo(p)    << std::setw(6) << Feature<Color::white>::tempo(p)    << std::setw(6) << Delta<Color::black>::tempo(p)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<Color::black>::center(p)   << std::setw(6) << Feature<Color::white>::center(p)   << std::setw(6) << Delta<Color::black>::center(p)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<Color::black>::balance(p)  << std::setw(6) << Feature<Color::white>::balance(p)  << std::setw(6) << Delta<Color::black>::balance(p)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<Color::black>::mobility(p) << std::setw(6) << Feature<Color::white>::mobility(p) << std::setw(6) << Delta<Color::black>::mobility(p) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << score<Color::black>(p) << std::setw(6) << score<Color::white>(p) << std::setw(6) << Delta<Color::black>(p) << '\n';
        std::cout<< '\n';
}

}       // namespace evaluate
}       // namespace dctl
