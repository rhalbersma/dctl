#include <dctl/move.hpp>
#include <dctl/position.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iomanip>
#include <iostream>
#include <stack>

// customize rules and board form these headers
#include <dctl/rules.hpp>
#include <dctl/board/types.hpp>

using namespace dctl;

int main()
{
        using Rules = international::Rules;
        using Board = board::International;
        using Pos = Position<Rules, Board>;

        //auto initial = Pos::initial();

        auto const initial = setup::read<international::Rules, board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        );

        std::stack<Pos> game;
        game.push(initial);

        while (true) {
                auto const p = game.top();
                std::cout << p;

                auto moves = successor::generate(p);
                boost::sort(moves, [](auto const& L, auto const& R) {
                        return move::str_numeric(L) < move::str_numeric(R);
                });

                if (moves.empty()) {
                        std::cout << "\nNo valid moves (Undo or stop [U/s]): ";
                } else {
                        auto index = 0;
                        for (auto&& m : moves)
                                std::cout << std::setw(2) << index++ << "." << m << '\n';
                        std::cout << "\nEnter move number, undo or stop [0/u/s]): ";
                }

                int choice = 0;

                for (std::string input; std::getline(std::cin, input);) {
                        boost::transform(input, begin(input), ::tolower);

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
