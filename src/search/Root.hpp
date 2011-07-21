#include <iostream>
#include <iomanip>
#include "Parameters.h"
#include "../evaluate/Evaluate.h"
#include "../generate/Successors.h"
#include "../move/String.h"
#include "../node/Layout.h"
#include "../node/Position.h"
#include "../node/Stack.h"
#include "../node/String.h"
#include "../utils/Timer.h"

namespace dtl {
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
        std::cout << std::endl;
        std::cout << layout::write<FEN_tag>()(p) << std::endl;
        std::cout << string::write<FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << depth << std::endl;
        std::cout << std::endl;
}

template<typename Rules, typename Board>
void Root::insert_PV(const Position<Board>& p, const Sequence& pv, int value)
{
        auto q(p);

        for (size_t i = 0; i < pv.size(); ++i) {
                Stack move_stack;
                generate::Successors<Rules, Board>::generate(q, move_stack);

                TT.insert(q, Transposition(value, Transposition::exact(), pv.size() - i, pv[i]));
                value = -score::stretch(value);

                q.template make<Rules>(move_stack[pv[i]]);
        }
        TT.insert(q, Transposition(value, Transposition::exact(), 0, Transposition::no_move()));
        
        assert(
                (value == Evaluate::evaluate(q)) || 
                (value == score::loss_value(0) && !generate::Successors<Rules, Board>::detect(q))
                // NOTE: with endgame databases, delayed losses can occur at the tips of the PV
        );
}

template<typename Rules, typename Board>
void Root::print_PV(const Position<Board>& p, const Sequence& pv)
{
        auto q(p);

        for (size_t i = 0; i < pv.size(); ++i) {
                Stack move_stack;
                generate::Successors<Rules, Board>::generate(q, move_stack);

                if (!(i % 2))                        
                        std::cout << std::setw(2) << std::right << ((i / 2) + 1) << ". ";
                std::cout << move::string::write<Rules>()(q, move_stack[pv[i]]);
                q.template make<Rules>(move_stack[pv[i]]);
                //if (q.same_king_moves(!q.to_move()))
                        //std::cout << "^" << q.same_king_moves(!q.to_move());
                if (i % 10 == 9)
                        std::cout << std::endl;
                else
                        std::cout << " ";

        }
        std::cout << std::endl << std::endl;
}

}       // namespace search
}       // namespace dtl
