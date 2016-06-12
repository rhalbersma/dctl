#pragma once
#include <dctl/aima/eval/delta.hpp>
#include <dctl/aima/eval/feature.hpp>
#include <dctl/aima/eval/weight.hpp>
#include <dctl/color.hpp>
#include <iomanip>
#include <iostream>

namespace dctl {
namespace evaluate {

template<class State>
static int score (State const& s)
{
        return (s.is_to_move(Color::black)) ?
                Delta<Color::black>::value(s) :
                Delta<Color::white>::value(s)
        ;
}

template<class State>
static void print_break_down(State const& s)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Action  " << std::setw(6) << Feature<Color::black>::material(s) << std::setw(6) << Feature<Color::white>::material(s) << std::setw(6) << Delta<Color::black>::material(s) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<Color::black>::tempo(s)    << std::setw(6) << Feature<Color::white>::tempo(s)    << std::setw(6) << Delta<Color::black>::tempo(s)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<Color::black>::center(s)   << std::setw(6) << Feature<Color::white>::center(s)   << std::setw(6) << Delta<Color::black>::center(s)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<Color::black>::balance(s)  << std::setw(6) << Feature<Color::white>::balance(s)  << std::setw(6) << Delta<Color::black>::balance(s)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<Color::black>::mobility(s) << std::setw(6) << Feature<Color::white>::mobility(s) << std::setw(6) << Delta<Color::black>::mobility(s) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << score<Color::black>(s) << std::setw(6) << score<Color::white>(s) << std::setw(6) << Delta<Color::black>(s) << '\n';
        std::cout<< '\n';
}

}       // namespace evaluate
}       // namespace dctl
