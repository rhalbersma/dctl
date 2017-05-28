#include <dctl/core/action.hpp>
#include <dctl/core/actions.hpp>
#include <dctl/algo/node.hpp>
#include <dctl/core/setup/string.hpp>
#include <dctl/core/state.hpp>
#include <boost/range/algorithm.hpp>    // sort, transform
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>

// customize rules and board from these headers
#include <dctl/core/rules.hpp>
#include <dctl/core/board.hpp>

using namespace dctl;

int main()
{
        using Rules = rules::international;
        using Board = board::mini;
        using State = state<Rules, Board>;

        auto initial = State::initial();// setup::read<Rules, Board, pdn::protocol>()("W:B1,5,6,9,10:W11,12,14,15,16");
        //("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46");

        std::stack<State> game;
        game.push(initial);

        while (true) {
                auto const state = game.top();
                std::cout << diag << state;
                std::cout << fen << state;
                //std::cout << "[" << p.reversible_actions() << "]\n";

                std::vector<action<Rules, Board>> actions;
                Actions<>{}.generate(state, actions);
                boost::sort(actions, [](auto const& lhs, auto const& rhs) {
                        return move::str_numeric(lhs) < move::str_numeric(rhs);
                });

                if (actions.empty()) {
                        std::cout << "\nNo valid moves (Undo or stop [u/s]): ";
                } else {
                        auto index = 0;
                        for (auto const& a : actions)
                                std::cout << std::setw(2) << index++ << ". " << a << '\n';
                        std::cout << "\nEnter move number, undo or stop [0/u/s]): ";
                }

                int choice = 0;

                for (std::string input; std::getline(std::cin, input);) {
                        boost::transform(input, input.begin(), ::tolower);

                        if (actions.empty()) {
                                if (input.empty() || input == "u")
                                        choice = -2;
                                else if (input == "s")
                                        choice = -1;
                                else {
                                        std::cout << "Invalid input\n";
                                        continue;
                                }
                        } else {
                                std::stringstream sstr(input);
                                if (!(sstr >> choice)) {
                                        if (input.empty())
                                                choice = 0;
                                        else if (input == "s")
                                                choice = -1;
                                        else if (input == "u")
                                                choice = -2;
                                        else {
                                                std::cout << "Invalid input\n";
                                                continue;
                                        }
                                }
                        }
                        break;
                }

                if (choice == -1)
                        break;
                if (choice == -2) {
                        if (game.size() > 1)
                                game.pop();
                        else
                                std::cout << "Cannot undo from the initial position!\n";
                        continue;
                }

                auto const a = actions[static_cast<std::size_t>(choice)];
                game.push(aima::result(state, a));
        }
}
