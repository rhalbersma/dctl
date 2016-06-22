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
        return (s.is_to_move(color::black)) ?
                Delta<color::black>::value(s) :
                Delta<color::white>::value(s)
        ;
}

template<class State>
static void print_break_down(State const& s)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Action  " << std::setw(6) << Feature<color::black>::material(s) << std::setw(6) << Feature<color::white>::material(s) << std::setw(6) << Delta<color::black>::material(s) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<color::black>::tempo(s)    << std::setw(6) << Feature<color::white>::tempo(s)    << std::setw(6) << Delta<color::black>::tempo(s)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<color::black>::center(s)   << std::setw(6) << Feature<color::white>::center(s)   << std::setw(6) << Delta<color::black>::center(s)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<color::black>::balance(s)  << std::setw(6) << Feature<color::white>::balance(s)  << std::setw(6) << Delta<color::black>::balance(s)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<color::black>::mobility(s) << std::setw(6) << Feature<color::white>::mobility(s) << std::setw(6) << Delta<color::black>::mobility(s) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << score<color::black>(s) << std::setw(6) << score<color::white>(s) << std::setw(6) << Delta<color::black>(s) << '\n';
        std::cout<< '\n';
}

}       // namespace evaluate
}       // namespace dctl
