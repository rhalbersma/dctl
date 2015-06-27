#pragma once
#include <dctl/player.hpp>
#include <dctl/eval/delta.hpp>
#include <dctl/eval/feature.hpp>
#include <dctl/eval/weight.hpp>
#include <iomanip>
#include <iostream>

namespace dctl {
namespace evaluate {

template<class State>
static int score (State const& p)
{
        return (p.to_move() == Player::black) ?
                Delta<Player::black>::value(p) :
                Delta<Player::white>::value(p)
        ;
}

template<class State>
static void print_break_down(State const& p)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Action    " << std::setw(6) << Feature<Player::black>::material(p) << std::setw(6) << Feature<Player::white>::material(p) << std::setw(6) << Delta<Player::black>::material(p) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<Player::black>::tempo(p)    << std::setw(6) << Feature<Player::white>::tempo(p)    << std::setw(6) << Delta<Player::black>::tempo(p)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<Player::black>::center(p)   << std::setw(6) << Feature<Player::white>::center(p)   << std::setw(6) << Delta<Player::black>::center(p)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<Player::black>::balance(p)  << std::setw(6) << Feature<Player::white>::balance(p)  << std::setw(6) << Delta<Player::black>::balance(p)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<Player::black>::mobility(p) << std::setw(6) << Feature<Player::white>::mobility(p) << std::setw(6) << Delta<Player::black>::mobility(p) << '\n';
        for (auto i = 0; i < 26; ++i)
                std::cout << "-";
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << score<Player::black>(p) << std::setw(6) << score<Player::white>(p) << std::setw(6) << Delta<Player::black>(p) << '\n';
        std::cout<< '\n';
}

}       // namespace evaluate
}       // namespace dctl
