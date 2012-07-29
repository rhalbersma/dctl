#pragma once
#include <iomanip>
#include <iostream>
#include "Delta.hpp"
#include "Feature.hpp"
#include "Weight.hpp"

namespace dctl {
namespace evaluate {

template<typename Position>
static int score (Position const& p)
{
        return (p.active_color() == Side::white)?
                Delta<Side::white>::value(p) :
                Delta<Side::black>::value(p)
        ;
}

template<typename Position>
static void print_break_down(Position const& p)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Move    " << std::setw(6) << Features<Side::black>::material(p) << std::setw(6) << Features<Side::white>::material(p) << std::setw(6) << Delta<Side::black, Rules, Board>::material(p) << "\n";
        std::cout << "Tempo   " << std::setw(6) << Features<Side::black>::tempo(p)    << std::setw(6) << Features<Side::white>::tempo(p)    << std::setw(6) << Delta<Side::black, Rules, Board>::tempo(p)    << "\n";
        std::cout << "Center  " << std::setw(6) << Features<Side::black>::center(p)   << std::setw(6) << Features<Side::white>::center(p)   << std::setw(6) << Delta<Side::black, Rules, Board>::center(p)   << "\n";
        std::cout << "Balance " << std::setw(6) << Features<Side::black>::balance(p)  << std::setw(6) << Features<Side::white>::balance(p)  << std::setw(6) << Delta<Side::black, Rules, Board>::balance(p)  << "\n";
        std::cout << "Mobility" << std::setw(6) << Features<Side::black>::mobility(p) << std::setw(6) << Features<Side::white>::mobility(p) << std::setw(6) << Delta<Side::black, Rules, Board>::mobility(p) << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Evaluate" << std::setw(6) << score<Side::black>(p) << std::setw(6) << score<Side::white>(p) << std::setw(6) << delta_evaluate<Side::black>(p) << "\n";
        std::cout<< "\n";
}

}       // namespace evaluate
}       // namespace dctl
