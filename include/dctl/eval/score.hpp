#pragma once
#include <dctl/eval/delta.hpp>
#include <dctl/eval/feature.hpp>
#include <dctl/eval/weight.hpp>
#include <dctl/core/state/color_piece.hpp>
#include <iomanip>
#include <iostream>

namespace dctl::eval {

template<class State>
static int score (State const& s)
{
        return s.to_move() == core::color::black ?
                Delta<core::black_>::value(s) :
                Delta<core::white_>::value(s)
        ;
}

template<class State>
[[maybe_unused]] static void print_break_down(State const& s)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Action  " << std::setw(6) << Feature<core::black_>::material(s) << std::setw(6) << Feature<core::white_>::material(s) << std::setw(6) << Delta<core::black_>::material(s) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<core::black_>::tempo(s)    << std::setw(6) << Feature<core::white_>::tempo(s)    << std::setw(6) << Delta<core::black_>::tempo(s)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<core::black_>::center(s)   << std::setw(6) << Feature<core::white_>::center(s)   << std::setw(6) << Delta<core::black_>::center(s)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<core::black_>::balance(s)  << std::setw(6) << Feature<core::white_>::balance(s)  << std::setw(6) << Delta<core::black_>::balance(s)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<core::black_>::mobility(s) << std::setw(6) << Feature<core::white_>::mobility(s) << std::setw(6) << Delta<core::black_>::mobility(s) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << Feature<core::black_>::value(s)    << std::setw(6) << Feature<core::white_>::value(s)    << std::setw(6) << Delta<core::black_>::value(s) << '\n';
        std::cout<< '\n';
}

}       // namespace dctl::eval
