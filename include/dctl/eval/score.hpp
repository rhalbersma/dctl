#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/eval/delta.hpp>
#include <dctl/eval/feature.hpp>
#include <dctl/eval/weight.hpp>
#include <dctl/core/state/color.hpp>
#include <iomanip>
#include <iostream>

namespace dctl::eval {

constexpr auto score(auto const& s)
{
        return s.to_move() == core::color::black ?
                Delta<core::black_>::value(s) :
                Delta<core::white_>::value(s)
        ;
}

[[maybe_unused]] constexpr auto print_break_down(auto const& s)
{
        std::cout << "Term    " << " BLACK"                                                                         << " WHITE"                                         << " DELTA"                                                      << '\n';
        for (auto i = 0; i < 26; ++i) {
                std::cout << "-";
        }
        std::cout<< '\n';
        std::cout << "Action  " << std::setw(6) << Feature<core::black_>::material(s) << std::setw(6) << Feature<core::white_>::material(s) << std::setw(6) << Delta<core::black_>::material(s) << '\n';
        std::cout << "Tempo   " << std::setw(6) << Feature<core::black_>::tempo(s)    << std::setw(6) << Feature<core::white_>::tempo(s)    << std::setw(6) << Delta<core::black_>::tempo(s)    << '\n';
        std::cout << "Center  " << std::setw(6) << Feature<core::black_>::center(s)   << std::setw(6) << Feature<core::white_>::center(s)   << std::setw(6) << Delta<core::black_>::center(s)   << '\n';
        std::cout << "Balance " << std::setw(6) << Feature<core::black_>::balance(s)  << std::setw(6) << Feature<core::white_>::balance(s)  << std::setw(6) << Delta<core::black_>::balance(s)  << '\n';
        std::cout << "Mobility" << std::setw(6) << Feature<core::black_>::mobility(s) << std::setw(6) << Feature<core::white_>::mobility(s) << std::setw(6) << Delta<core::black_>::mobility(s) << '\n';
        for (auto i = 0; i < 26; ++i) {
                std::cout << "-";
        }
        std::cout<< '\n';
        std::cout << "Evaluate" << std::setw(6) << Feature<core::black_>::value(s)    << std::setw(6) << Feature<core::white_>::value(s)    << std::setw(6) << Delta<core::black_>::value(s) << '\n';
        std::cout<< '\n';
}

}       // namespace dctl::eval
