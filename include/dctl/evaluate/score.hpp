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
        return (p.to_move() == Side::black)?
                Delta<Side::black>::value(p) :
                Delta<Side::white>::value(p)
        ;
}

template<class Position>
static void print_break_down(Position const& p)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Move    " << std::setw(6) << Feature<Side::black>::material(p) << std::setw(6) << Feature<Side::white>::material(p) << std::setw(6) << Delta<Side::black>::material(p) << "\n";
        std::cout << "Tempo   " << std::setw(6) << Feature<Side::black>::tempo(p)    << std::setw(6) << Feature<Side::white>::tempo(p)    << std::setw(6) << Delta<Side::black>::tempo(p)    << "\n";
        std::cout << "Center  " << std::setw(6) << Feature<Side::black>::center(p)   << std::setw(6) << Feature<Side::white>::center(p)   << std::setw(6) << Delta<Side::black>::center(p)   << "\n";
        std::cout << "Balance " << std::setw(6) << Feature<Side::black>::balance(p)  << std::setw(6) << Feature<Side::white>::balance(p)  << std::setw(6) << Delta<Side::black>::balance(p)  << "\n";
        std::cout << "Mobility" << std::setw(6) << Feature<Side::black>::mobility(p) << std::setw(6) << Feature<Side::white>::mobility(p) << std::setw(6) << Delta<Side::black>::mobility(p) << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Evaluate" << std::setw(6) << score<Side::black>(p) << std::setw(6) << score<Side::white>(p) << std::setw(6) << Delta<Side::black>(p) << "\n";
        std::cout<< "\n";
}

}       // namespace evaluate
}       // namespace dctl
