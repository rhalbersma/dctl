#pragma once
#include <iomanip>
#include <iostream>
#include "Delta.hpp"
#include "Feature.hpp"
#include "Weight.hpp"
#include "../node/Position.hpp"

namespace dctl { 
        
template<typename> struct Position;

template
<
        typename Rules
>
class Evaluate
{
public:
        template<typename Board> 
        static int evaluate(const Position<Board>& p)
        {
                return (p.active_color() == Side::white)? 
                        Delta<Side::white, Rules, Board>::evaluate(p) : 
                        Delta<Side::black, Rules, Board>::evaluate(p)
                ;
        }
        
        template<typename Board> 
        static void print_break_down(const Position<Board>& p)
        {
                std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << "\n";
                for (auto i = 0; i < 26; ++i)
                        std::cout << "-";
                std::cout<< "\n";
                std::cout << "Move    " << std::setw(6) << Features<Side::black, Rules, Board>::material(p) << std::setw(6) << Features<Side::white, Rules, Board>::material(p) << std::setw(6) << Delta<Side::black, Rules, Board>::material(p) << "\n";
                std::cout << "Tempo   " << std::setw(6) << Features<Side::black, Rules, Board>::tempo(p)    << std::setw(6) << Features<Side::white, Rules, Board>::tempo(p)    << std::setw(6) << Delta<Side::black, Rules, Board>::tempo(p)    << "\n";
                std::cout << "Center  " << std::setw(6) << Features<Side::black, Rules, Board>::center(p)   << std::setw(6) << Features<Side::white, Rules, Board>::center(p)   << std::setw(6) << Delta<Side::black, Rules, Board>::center(p)   << "\n";
                std::cout << "Balance " << std::setw(6) << Features<Side::black, Rules, Board>::balance(p)  << std::setw(6) << Features<Side::white, Rules, Board>::balance(p)  << std::setw(6) << Delta<Side::black, Rules, Board>::balance(p)  << "\n";
                std::cout << "Mobility" << std::setw(6) << Features<Side::black, Rules, Board>::mobility(p) << std::setw(6) << Features<Side::white, Rules, Board>::mobility(p) << std::setw(6) << Delta<Side::black, Rules, Board>::mobility(p) << "\n";
                for (auto i = 0; i < 26; ++i)
                        std::cout << "-";
                std::cout<< "\n";
                std::cout << "Evaluate" << std::setw(6) << evaluate<Side::black>(p) << std::setw(6) << evaluate<Side::white>(p) << std::setw(6) << delta_evaluate<Side::black>(p) << "\n";
                std::cout<< "\n";
        }
};

}       // namespace dctl
