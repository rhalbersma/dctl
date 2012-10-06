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
        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;

        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Move    " << std::setw(6) << Feature<Side::black>::material(p) << std::setw(6) << Feature<Side::white>::material(p) << std::setw(6) << Delta<Side::black, Rules, Board>::material(p) << "\n";
        std::cout << "Tempo   " << std::setw(6) << Feature<Side::black>::tempo(p)    << std::setw(6) << Feature<Side::white>::tempo(p)    << std::setw(6) << Delta<Side::black, Rules, Board>::tempo(p)    << "\n";
        std::cout << "Center  " << std::setw(6) << Feature<Side::black>::center(p)   << std::setw(6) << Feature<Side::white>::center(p)   << std::setw(6) << Delta<Side::black, Rules, Board>::center(p)   << "\n";
        std::cout << "Balance " << std::setw(6) << Feature<Side::black>::balance(p)  << std::setw(6) << Feature<Side::white>::balance(p)  << std::setw(6) << Delta<Side::black, Rules, Board>::balance(p)  << "\n";
        std::cout << "Mobility" << std::setw(6) << Feature<Side::black>::mobility(p) << std::setw(6) << Feature<Side::white>::mobility(p) << std::setw(6) << Delta<Side::black, Rules, Board>::mobility(p) << "\n";
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< "\n";
        std::cout << "Evaluate" << std::setw(6) << score<Side::black>(p) << std::setw(6) << score<Side::white>(p) << std::setw(6) << Delta<Side::black>(p) << "\n";
        std::cout<< "\n";
}

}       // namespace evaluate
}       // namespace dctl
