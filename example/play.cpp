//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/node.hpp>
#include <dctl/core.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>

using namespace dctl::core;
using namespace dctl::algo;

int main()
{
        using Rules = international;
        using Board = basic_board<international>;
        using State = basic_state<Rules, Board>;

        auto initial = pdn::read<Rules, Board>{}("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46");

        std::stack<State> game;
        game.push(initial);

        while (true) {
                auto const state = game.top();
                std::cout << diag << state;
                std::cout << fen << state;
                //std::cout << "[" << p.reversible_actions() << "]\n";

                auto actions = drop_duplicates_gen.generate(state);
                std::ranges::sort(actions, [](auto const& lhs, auto const& rhs) {
                        return str_numeric(lhs) < str_numeric(rhs);
                });

                if (actions.empty()) {
                        std::cout << "\nNo valid moves (Undo or stop [u/s]): ";
                } else {
                        auto index = 0;
                        for (auto const& a : actions) {
                                std::cout << std::setw(2) << index++ << ". " << a << '\n';
                        }
                        std::cout << "\nEnter move number, undo or stop [0/u/s]): ";
                }

                int choice = 0;

                for (std::string input; std::getline(std::cin, input);) {
                        std::ranges::transform(input, input.begin(), ::tolower);

                        if (actions.empty()) {
                                if (input.empty() || input == "u") {
                                        choice = -2;
                                } else if (input == "s") {
                                        choice = -1;
                                } else {
                                        std::cout << "Invalid input\n";
                                        continue;
                                }
                        } else {
                                std::stringstream sstr(input);
                                if (!(sstr >> choice)) {
                                        if (input.empty()) {
                                                choice = 0;
                                        } else if (input == "s") {
                                                choice = -1;
                                        } else if (input == "u") {
                                                choice = -2;
                                        } else {
                                                std::cout << "Invalid input\n";
                                                continue;
                                        }
                                }
                        }
                        break;
                }

                if (choice == -1) {
                        break;
                }
                if (choice == -2) {
                        if (game.size() > 1) {
                                game.pop();
                        } else {
                                std::cout << "Cannot undo from the initial position!\n";
                        }
                        continue;
                }

                auto const a = actions[static_cast<std::size_t>(choice)];
                game.push(result(state, a));
        }
}
