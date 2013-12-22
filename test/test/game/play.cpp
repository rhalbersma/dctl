#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>
#include <dctl/board/types.hpp>
#include <dctl/node/position.hpp>
#include <dctl/move/ostream.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/utility/stack_vector.hpp>

using namespace dctl;

int main()
{
        using Rules = rules::International;
        using Board = board::International;
        using Pos = Position<Rules, Board>;

        auto initial = Pos::initial();
        std::stack<Pos> game;
        game.push(initial);

        while (true) {
                auto const p = game.top();
                std::cout << setup::diagram<pdn::protocol>()(p);

                auto moves = successor::generate(p);
                std::sort(begin(moves), end(moves), [](auto const& L, auto const& R) {
                        return L.numeric() < R.numeric();
                });

                if (moves.empty()) {
                        std::cout << "\nNo valid moves (-1 to stop / -2 to undo): ";
                } else {
                        auto index = 0;
                        for (auto const& m : moves)
                                std::cout << std::setw(2) << index++ << "." << m << '\n';
                        std::cout << "\nEnter move number (-1 to stop / -2 to undo): ";
                }

                int choice;
                std::cin >> choice;

                if (choice == -1) break;
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
