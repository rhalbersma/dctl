#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>
#include <dctl/position/position.hpp>
#include <dctl/move/ostream.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/utility/stack_vector.hpp>

// customize rules and board form these headers
#include <dctl/rules/variants.hpp>
#include <dctl/board/types.hpp>

using namespace dctl;

int main()
{
        using Rules = rules::International;
        using Board = board::International;
        using Pos = Position<Rules, Board>;

        auto initial = setup::read<rules::International, board::International, pdn::protocol>()(
                        "W:WK46, 28,14:B1"
                );//Pos::initial();
        std::stack<Pos> game;
        game.push(initial);

        while (true) {
                auto const p = game.top();
                std::cout << setup::diagram<pdn::protocol>()(p);

                auto moves = successor::generate(p);
                std::sort(begin(moves), end(moves), [](auto const& L, auto const& R) {
                        return format::as_numeric(L) < format::as_numeric(R);
                });

                if (moves.empty()) {
                        std::cout << "\nNo valid moves (Undo or stop [U/s]): ";
                } else {
                        auto index = 0;
                        for (auto const& m : moves)
                                std::cout << std::setw(2) << index++ << "." << m << '\n';
                        std::cout << "\nEnter move number, undo or stop [0/u/s]): ";
                }

                int choice = 0;

                for (std::string input; std::getline(std::cin, input);) {
                        std::transform(begin(input), end(input), begin(input), ::tolower);

                        if (moves.empty()) {
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

                auto const m = moves[static_cast<std::size_t>(choice)];
                game.push(successor::make_copy(p, m));
        }
}
