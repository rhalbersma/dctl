#pragma once
#include <dctl/eval/delta.hpp>
#include <dctl/eval/feature.hpp>
#include <dctl/eval/weight.hpp>
#include <dctl/core/color_piece.hpp>
#include <iomanip>
#include <iostream>

namespace dctl {
namespace evaluate {

template<class State>
static int score (State const& s)
{
        return s.to_move() == color::black ?
                Delta<black_>::value(s) :
                Delta<white_>::value(s)
        ;
}

template<class State>
static void print_break_down(State const& s)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Action  " << std::setw(6) << Feature<black_>::material(s) << std::setw(6) << Feature<white_>::material(s) << std::setw(6) << Delta<black_>::material(s) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<black_>::tempo(s)    << std::setw(6) << Feature<white_>::tempo(s)    << std::setw(6) << Delta<black_>::tempo(s)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<black_>::center(s)   << std::setw(6) << Feature<white_>::center(s)   << std::setw(6) << Delta<black_>::center(s)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<black_>::balance(s)  << std::setw(6) << Feature<white_>::balance(s)  << std::setw(6) << Delta<black_>::balance(s)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<black_>::mobility(s) << std::setw(6) << Feature<white_>::mobility(s) << std::setw(6) << Delta<black_>::mobility(s) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << Feature<black_>::value(s)    << std::setw(6) << Feature<white_>::value(s)    << std::setw(6) << Delta<black_>::value(s) << '\n';
        std::cout<< '\n';
}

}       // namespace evaluate
}       // namespace dctl
