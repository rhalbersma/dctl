#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>
#include <dctl/board/types.hpp>
#include <dctl/node/position.hpp>
#include <dctl/notation/string.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/successor/copy.hpp>
#include <dctl/utility/stack_vector.hpp>

using namespace dctl;

int main()
{
        using Rules = rules::International;
        using Board = board::International;
        auto initial = Position<Rules, Board>::initial();
        std::stack<Position<Rules, Board>> game;
        game.push(initial);

        while (true) {
                auto const p = game.top();
                std::cout << setup::diagram<pdn::protocol>()(p);

                Arena<Move<Rules, Board>> a;
                auto moves = successor::copy(p, Alloc<Move<Rules, Board>>{a});
                std::sort(begin(moves), end(moves), [=](auto L, auto R) {
                        return notation::write(p, L) < notation::write(p, R);
                });

                if (moves.empty()) {
                        std::cout << "\nNo valid moves (-1 to stop / -2 to undo): ";
                } else {
                        auto index = 0;
                        for (auto const& m : moves)
                                std::cout << std::setw(2) << index++ << "." << notation::write(p, m) << "\n";
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
