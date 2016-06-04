#include <dctl/action.hpp>
#include <dctl/actions.hpp>
#include <dctl/aima/node.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/state.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>

// customize rules and board from these headers
#include <dctl/rules.hpp>
#include <dctl/board.hpp>

using namespace dctl;

int main()
{
        using Rules = rules::international;
        using Board = board::international;
        using State = State<Rules, Board>;

        auto initial = setup::read<Rules, Board, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        );//        State::initial();

        std::stack<State> game;
        game.push(initial);

        while (true) {
                auto const state = game.top();
                std::cout << diag << state;
                std::cout << fen << state;
                //std::cout << "[" << p.reversible_actions() << "]\n";

                std::vector<Action<Rules, Board>> actions;
                core::Actions<>{}.generate(state, actions);
                std::sort(actions.begin(), actions.end(), [](auto const& lhs, auto const& rhs) {
                        return move::str_numeric(lhs) < move::str_numeric(rhs);
                });

                if (actions.empty()) {
                        std::cout << "\nNo valid moves (Undo or stop [U/s]): ";
                } else {
                        auto index = 0;
                        for (auto const& a : actions)
                                std::cout << std::setw(2) << index++ << ". " << a << '\n';
                        std::cout << "\nEnter move number, undo or stop [0/u/s]): ";
                }

                int choice = 0;

                for (std::string input; std::getline(std::cin, input);) {
                        std::transform(input.cbegin(), input.cend(), input.begin(), ::tolower);

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
