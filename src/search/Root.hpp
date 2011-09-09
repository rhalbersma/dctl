#include <iostream>
#include <iomanip>
#include "Parameters.h"
#include "../evaluate/Evaluate.h"
#include "../successor/Successor.h"
#include "../node/Position.h"
#include "../node/Stack.h"
#include "../utils/Timer.h"

#include "../setup/Diagram.h"
#include "../setup/String.h"
#include "../notation/String.h"

namespace dctl {
namespace search {

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::analyze(const Position<Board>& p, int depth)
{
        return iterative_deepening<Rules, Board>(p, depth);
}

template<typename Board>
void Root::announce(const Position<Board>& p, int depth)
{
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << setup::write<pdn::protocol>()(p) << "\n";
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

// UCI format
template<typename Rules, typename Board>
void Root::report(int depth, int value, const Timer& timer, const Position<Board>& p, const Sequence& pv)
{
        std::cout << "info";

        std::cout << " score ";
        std::cout << std::setw( 3) << std::right << print(value);

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " nodes ";
        std::cout << std::setw(11) << std::right << statistics_.nodes();

        std::cout << " time ";
        std::cout << std::setw( 6) << timer.elapsed();

        const double nps = (1000 * statistics_.nodes()) / static_cast<double>(timer.lap());
        std::cout << " nps ";
        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
        std::cout << std::setw( 7) << nps;

        const double hashfull = 1000 * (static_cast<double>((TT.size() - TT.available())) / TT.size());
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;

        std::cout << "\n";
        print_PV<Rules>(p, pv);
}

template<typename Rules, typename Board>
void Root::insert_PV(const Position<Board>& p, const Sequence& pv, int value)
{
        auto q = p;

        for (auto i = 0; i < static_cast<int>(pv.size()); ++i) {
                Stack move_stack;
                Successor<successor::Legal, Rules>::generate(q, move_stack);

                TT.insert(q, Transposition(value, Transposition::exact_value, pv.size() - i, pv[i]));
                value = -stretch(value);

                q.template make<Rules>(move_stack[pv[i]]);
        }
        TT.insert(q, Transposition(value, Transposition::exact_value, 0, Transposition::no_move()));
        
        BOOST_ASSERT(
                (value == Evaluate::evaluate(q)) || 
                (value == loss_value(0) && !Successor<successor::Legal, Rules>::detect(q))
                // NOTE: with endgame databases, delayed losses can occur at the tips of the PV
        );
}

template<typename Rules, typename Board>
void Root::print_PV(const Position<Board>& p, const Sequence& pv)
{
        auto q = p;

        for (auto i = 0; i < static_cast<int>(pv.size()); ++i) {
                Stack moves;
                Successor<successor::Legal, Rules>::generate(q, moves);

                if (!(i % 2))                        
                        std::cout << std::setw(2) << std::right << ((i / 2) + 1) << ". ";
                std::cout << notation::write<Rules>()(q, moves[pv[i]]);
                q.template make<Rules>(moves[pv[i]]);
                //if (q.same_king_moves(!q.to_move()))
                        //std::cout << "^" << q.same_king_moves(!q.to_move());
                if (i % 10 == 9)
                        std::cout << "\n";
                else
                        std::cout << " ";

        }
        std::cout << "\n\n";
}

}       // namespace search
}       // namespace dctl
