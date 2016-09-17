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
                Delta<black_type>::value(s) :
                Delta<white_type>::value(s)
        ;
}

template<class State>
static void print_break_down(State const& s)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Action  " << std::setw(6) << Feature<black_type>::material(s) << std::setw(6) << Feature<white_type>::material(s) << std::setw(6) << Delta<black_type>::material(s) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<black_type>::tempo(s)    << std::setw(6) << Feature<white_type>::tempo(s)    << std::setw(6) << Delta<black_type>::tempo(s)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<black_type>::center(s)   << std::setw(6) << Feature<white_type>::center(s)   << std::setw(6) << Delta<black_type>::center(s)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<black_type>::balance(s)  << std::setw(6) << Feature<white_type>::balance(s)  << std::setw(6) << Delta<black_type>::balance(s)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<black_type>::mobility(s) << std::setw(6) << Feature<white_type>::mobility(s) << std::setw(6) << Delta<black_type>::mobility(s) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << score<Color::black>(s) << std::setw(6) << score<Color::white>(s) << std::setw(6) << Delta<black_type>(s) << '\n';
        std::cout<< '\n';
}

}       // namespace evaluate
}       // namespace dctl
